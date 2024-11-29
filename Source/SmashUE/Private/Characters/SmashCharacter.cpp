// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "Characters/SmashCharacterInputData.h"
#include "EnhancedInputComponent.h"
#include "SmashCharacterState.h"
#include "UsefulFunctions.h"
#include "Camera/CameraWorldSubsystem.h"
#include "Characters/SmashCharacterSettings.h"
#include "Components/CapsuleComponent.h"

ASmashCharacter::ASmashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void ASmashCharacter::BeginPlay()
{
	Super::BeginPlay();

	CreateStates();
	CreateStateMachine();
	InitStateMachine();

	GetWorld()->GetSubsystem<UCameraWorldSubsystem>()->AddFollowTarget(this);
}

void ASmashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickStateMachine(DeltaTime);
	RotateMeshUsingOrientX();
}

void ASmashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if(EnhancedInputComponent == nullptr)
	{
		return;
	}

	BindInputActions(EnhancedInputComponent);
}

float ASmashCharacter::GetOrientX() const
{
	return OrientX;
}

void ASmashCharacter::SetOrientX(float NewOrientX)
{
	if(GetDefault<USmashCharacterSettings>()->b2DOrient)
	{
		if(NewOrientX > 0.0f)
		{
			NewOrientX = 1;
		}
		else if(NewOrientX < 0.0f)
		{
			NewOrientX = -1.0;
		}
		else
		{
			NewOrientX = 1;
		}
	}

	OrientX = NewOrientX;
}

void ASmashCharacter::RotateMeshUsingOrientX() const
{
	FRotator Rotation = GetMesh()->GetRelativeRotation();
	Rotation.Yaw = -90.0f * OrientX;
	GetMesh()->SetRelativeRotation(Rotation);
}

void ASmashCharacter::CreateStateMachine()
{
	StateMachine = NewObject<USmashCharacterStateMachine>();
}

void ASmashCharacter::InitStateMachine()
{
	if(StateMachine == nullptr)
	{
		return;
	}

	StateMachine->Init(this);
}

void ASmashCharacter::TickStateMachine(float DeltaTime) const
{
	if(StateMachine == nullptr)
	{
		return;
	}

	StateMachine->Tick(DeltaTime);
}

const TArray<USmashCharacterState*>& ASmashCharacter::GetStates() const
{
	return States;
}

void ASmashCharacter::CreateStates()
{
	const USmashCharacterSettings* CharacterSettings = GetDefault<USmashCharacterSettings>();

	for (TTuple<ESmashCharacterStateID, TSubclassOf<USmashCharacterState>> State : CharacterSettings->CharacterStates)
	{
		if(State.Key == ESmashCharacterStateID::None)
		{
			continue;
		}

		USmashCharacterState* NewState = nullptr;

		if(CharacterStates.Contains(State.Key))
		{
			const TSubclassOf<USmashCharacterState> Class = *CharacterStates.Find(State.Key);

			if(Class != nullptr)
			{
				NewState = NewObject<USmashCharacterState>(this, Class);
			}
		}

		if(NewState == nullptr)
		{
			if(State.Value != nullptr)
			{
				NewState = NewObject<USmashCharacterState>(this, State.Value);
			}
		}

		States.Add(NewState);
	}
}

float ASmashCharacter::GetInputMoveX() const
{
	return InputMoveX;
}

float ASmashCharacter::GetInputMoveY() const
{
	return InputMoveY;
}

bool ASmashCharacter::GetInputJump() const
{
	return bInputJump;
}

bool ASmashCharacter::GetInputBasicAttack() const
{
	return bInputBasicAttack;
}

bool ASmashCharacter::GetInputSpecialAttack() const
{
	return bInputSpecialAttack;
}

void ASmashCharacter::BindInputActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if(InputData == nullptr)
	{
		return;
	}

	TArray<ETriggerEvent> DefaultTriggerEvents;
	DefaultTriggerEvents.Add(ETriggerEvent::Started);
	DefaultTriggerEvents.Add(ETriggerEvent::Triggered);
	DefaultTriggerEvents.Add(ETriggerEvent::Completed);

	if(InputData->InputActionMoveX != nullptr)
	{
		FName FuncName = GET_FUNCTION_NAME_CHECKED_OneParam(ASmashCharacter, OnInputMoveX, const FInputActionValue&);
		UUsefulFunctions::BindAction(EnhancedInputComponent, InputData->InputActionMoveX, DefaultTriggerEvents, this, FuncName);
	}

	if(InputData->InputActionMoveXFast != nullptr)
	{
		TArray<ETriggerEvent> TriggerEvents;
		TriggerEvents.Add(ETriggerEvent::Triggered);
		FName FuncName = GET_FUNCTION_NAME_CHECKED_OneParam(ASmashCharacter, OnInputMoveXFast, const FInputActionValue&);
		UUsefulFunctions::BindAction(EnhancedInputComponent, InputData->InputActionMoveXFast, TriggerEvents, this, FuncName);
	}

	if(InputData->InputActionMoveY != nullptr)
	{
		FName FuncName = GET_FUNCTION_NAME_CHECKED_OneParam(ASmashCharacter, OnInputMoveY, const FInputActionValue&);
		UUsefulFunctions::BindAction(EnhancedInputComponent, InputData->InputActionMoveY, DefaultTriggerEvents, this, FuncName);
	}

	if(InputData->InputActionJump != nullptr)
	{
		FName FuncName = GET_FUNCTION_NAME_CHECKED_OneParam(ASmashCharacter, OnInputJump, const FInputActionValue&);
		UUsefulFunctions::BindAction(EnhancedInputComponent, InputData->InputActionJump, DefaultTriggerEvents, this, FuncName);
	}

	if(InputData->InputActionBasicAttack != nullptr)
	{
		FName FuncName = GET_FUNCTION_NAME_CHECKED_OneParam(ASmashCharacter, OnInputBasicAttack, const FInputActionValue&);
		UUsefulFunctions::BindAction(EnhancedInputComponent, InputData->InputActionBasicAttack, DefaultTriggerEvents, this, FuncName);
	}

	if(InputData->InputActionSpecialAttack != nullptr)
	{
		FName FuncName = GET_FUNCTION_NAME_CHECKED_OneParam(ASmashCharacter, OnInputSpecialAttack, const FInputActionValue&);
		UUsefulFunctions::BindAction(EnhancedInputComponent, InputData->InputActionSpecialAttack, DefaultTriggerEvents, this, FuncName);
	}
}

void ASmashCharacter::OnInputMoveX(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
}

void ASmashCharacter::OnInputMoveY(const FInputActionValue& InputActionValue)
{
	InputMoveY = InputActionValue.Get<float>();
}

void ASmashCharacter::OnInputMoveXFast(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
	InputMoveXFastEvent.Broadcast(InputMoveX);
}

void ASmashCharacter::OnInputJump(const FInputActionValue& InputActionValue)
{
	bInputJump = InputActionValue.Get<bool>();
}

void ASmashCharacter::OnInputBasicAttack(const FInputActionValue& InputActionValue)
{
	bInputBasicAttack = InputActionValue.Get<bool>();
}

void ASmashCharacter::OnInputSpecialAttack(const FInputActionValue& InputActionValue)
{
	bInputSpecialAttack = InputActionValue.Get<bool>();
}

FVector ASmashCharacter::GetFollowPosition()
{
	return GetActorLocation();
}

bool ASmashCharacter::IsFollowable()
{
	return true;
}

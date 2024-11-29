// Fill out your copyright notice in the Description page of Project Settings.


#include "CharactersSelection/CharacterSelectionPawn.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "UsefulFunctions.h"
#include "CharactersSelection/CharacterSelectionInputData.h"

ACharacterSelectionPawn::ACharacterSelectionPawn()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACharacterSelectionPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterSelectionPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if(EnhancedInputComponent == nullptr)
	{
		return;
	}

	if(InputData == nullptr)
	{
		return;
	}

	TArray<ETriggerEvent> DefaultTriggerEvents;
	DefaultTriggerEvents.Add(ETriggerEvent::Started);
	DefaultTriggerEvents.Add(ETriggerEvent::Triggered);
	DefaultTriggerEvents.Add(ETriggerEvent::Completed);

	if(InputData->InputInteract)
	{
		FName FuncName = GET_FUNCTION_NAME_CHECKED_OneParam(ACharacterSelectionPawn, OnInputInteract, const FInputActionValue&);
		UUsefulFunctions::BindAction(EnhancedInputComponent, InputData->InputInteract, DefaultTriggerEvents, this, FuncName);
	}

	if(InputData->InputMove)
	{
		FName FuncName = GET_FUNCTION_NAME_CHECKED_OneParam(ACharacterSelectionPawn, OnInputMove, const FInputActionValue&);
		UUsefulFunctions::BindAction(EnhancedInputComponent, InputData->InputMove, DefaultTriggerEvents, this, FuncName);
	}

	if(InputData->InputStart)
	{
		FName FuncName = GET_FUNCTION_NAME_CHECKED_OneParam(ACharacterSelectionPawn, OnInputStart, const FInputActionValue&);
		UUsefulFunctions::BindAction(EnhancedInputComponent, InputData->InputStart, DefaultTriggerEvents, this, FuncName);
	}
}

void ACharacterSelectionPawn::SetInputData(UCharacterSelectionInputData* InInputData)
{
	InputData = InInputData;
}

void ACharacterSelectionPawn::OnInputInteract(const FInputActionValue& InputActionValue)
{
	bInputInteract = InputActionValue.Get<bool>();
}

void ACharacterSelectionPawn::OnInputMove(const FInputActionValue& InputActionValue)
{
	InputMove = InputActionValue.Get<FVector2D>();
}

void ACharacterSelectionPawn::OnInputStart(const FInputActionValue& InputActionValue)
{
	bInputStart = InputActionValue.Get<bool>();
}


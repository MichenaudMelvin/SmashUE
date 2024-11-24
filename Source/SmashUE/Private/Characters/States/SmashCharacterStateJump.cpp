// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateJump.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/States/SmashCharacterStateFall.h"

ESmashCharacterStateID USmashCharacterStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}

void USmashCharacterStateJump::StateInit(USmashCharacterStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);

	if(StateAnimation == nullptr)
	{
		bUseAnimDuration = false;
	}

	if(OtherJumpsAnimation == nullptr)
	{
		OtherJumpsAnimation = StateAnimation;
	}

	float TargetJumpDuration = bUseAnimDuration ? StateAnimation->GetPlayLength() : JumpDuration;

	JumpVelocity = (2.0 * JumpMaxHeight) / TargetJumpDuration;
	JumpGravity = (-2.0 * JumpMaxHeight) / FMath::Pow(TargetJumpDuration, 2);

	USmashCharacterStateFall* FallState = Cast<USmashCharacterStateFall>(StateMachine->GetState(ESmashCharacterStateID::Fall));
	if(FallState == nullptr)
	{
#if WITH_EDITOR
		const FString Message = FString::Printf(TEXT("Fall state not implemented in %s"), *Character.GetClass()->GetName());

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);
		FMessageLog("BlueprintLog").Warning(FText::FromString(Message));
#endif
		return;
	}

	FallState->SetMaxJumpCount(MaxJumpCount);
}

void USmashCharacterStateJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	JumpCount++;

	Super::StateEnter(PreviousStateID);

	Character->GetCharacterMovement()->MaxWalkSpeed = JumpWalkSpeed;
	Character->GetCharacterMovement()->AirControl = JumpAirControl;
	Character->GetCharacterMovement()->GravityScale = 0.0f;

	Character->GetCharacterMovement()->Velocity.Z = JumpVelocity;
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Falling);

	if(Character->GetInputMoveX() != 0.0f)
	{
		Character->SetOrientX(Character->GetInputMoveX());
	}

	Character->GetCharacterMovement()->Velocity.X = Character->GetOrientX() * FMath::Abs(Character->GetCharacterMovement()->Velocity.X);
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	if(NextStateID != ESmashCharacterStateID::Fall)
	{
		return;
	}

	USmashCharacterStateFall* FallState = Cast<USmashCharacterStateFall>(StateMachine->GetState(NextStateID));
	if(FallState == nullptr)
	{
		return;
	}

	FallState->SetJumpCount(JumpCount);
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	Character->GetCharacterMovement()->Velocity.Z += JumpGravity * DeltaTime;

	const float MoveXValue = Character->GetInputMoveX();

	if(FMath::Abs(MoveXValue) > CharacterSettings->InputMoveXThreshold)
	{
		Character->AddMovementInput(FVector::ForwardVector, MoveXValue);
	}

	if(!Character->GetInputJump() || Character->GetCharacterMovement()->Velocity.Z <= 0.0f)
	{
		Character->GetCharacterMovement()->Velocity.Z = 0.0f;
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
}

void USmashCharacterStateJump::PlayStateAnimation()
{
	UAnimMontage* AnimToPlay = JumpCount == 1 ? StateAnimation : OtherJumpsAnimation;

	if(AnimToPlay == nullptr)
	{
		Character->StopAnimMontage(nullptr);
		return;
	}

	Character->PlayAnimMontage(AnimToPlay);
}

#if WITH_EDITOR
void USmashCharacterStateJump::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName& PropertyName = PropertyChangedEvent.GetMemberPropertyName();

	if (PropertyName == GET_MEMBER_NAME_CHECKED(USmashCharacterStateJump, bUseAnimDuration) || PropertyName == GET_MEMBER_NAME_CHECKED(USmashCharacterStateJump, StateAnimation))
	{
		if(bUseAnimDuration && StateAnimation != nullptr)
		{
			JumpDuration = StateAnimation->GetPlayLength();
		}
	}
}
#endif
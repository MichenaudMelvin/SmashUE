// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateJump.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/SmashCharacterSettings.h"

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

	float TargetJumpDuration = bUseAnimDuration ? StateAnimation->GetPlayLength() : JumpDuration;

	JumpVelocity = (2.0 * JumpMaxHeight) / TargetJumpDuration;
	JumpGravity = (-2.0 * JumpMaxHeight) / FMath::Pow(TargetJumpDuration, 2);
}

void USmashCharacterStateJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->GetCharacterMovement()->MaxWalkSpeed = JumpWalkSpeed;
	Character->GetCharacterMovement()->AirControl = JumpAirControl;
	Character->GetCharacterMovement()->GravityScale = 0.0f;

	Character->GetCharacterMovement()->Velocity.Z = JumpVelocity;
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
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
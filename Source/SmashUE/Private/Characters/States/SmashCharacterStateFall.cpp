// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateFall.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/SmashCharacterSettings.h"

ESmashCharacterStateID USmashCharacterStateFall::GetStateID()
{
	return ESmashCharacterStateID::Fall;
}

void USmashCharacterStateFall::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->GetCharacterMovement()->MaxWalkSpeed = FallHorizontalMoveSpeed;
	Character->GetCharacterMovement()->AirControl = FallAirControl;
}

void USmashCharacterStateFall::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
}

void USmashCharacterStateFall::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	const float MoveXValue = Character->GetInputMoveX();
	const float MoveYValue = Character->GetInputMoveY();

	if(MoveYValue < -CharacterSettings->InputMoveXThreshold)
	{
		Character->GetCharacterMovement()->GravityScale = FallFastGravityScale;
	}
	else
	{
		Character->GetCharacterMovement()->GravityScale = FallGravityScale;
	}

	if(FMath::Abs(MoveXValue) > CharacterSettings->InputMoveXThreshold)
	{
		Character->AddMovementInput(FVector::ForwardVector, MoveXValue);
	}

	if(Character->GetCharacterMovement()->IsFalling())
	{
		return;
	}

	StateMachine->ChangeState(ESmashCharacterStateID::Idle);
}


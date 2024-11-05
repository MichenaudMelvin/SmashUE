// Fill out your copyright notice in the Description page of Project Settings.


#include "SmashCharacterState.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "Characters/SmashCharacter.h"

USmashCharacterState::USmashCharacterState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

ESmashCharacterStateID USmashCharacterState::GetStateID()
{
	return ESmashCharacterStateID::None;
}

void USmashCharacterState::StateInit(USmashCharacterStateMachine* InStateMachine)
{
	StateMachine = InStateMachine;
	Character = InStateMachine->GetCharacter();
}

void USmashCharacterState::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	if(StateAnimation == nullptr)
	{
		return;
	}

	Character->PlayAnimMontage(StateAnimation);
}

void USmashCharacterState::StateExit(ESmashCharacterStateID NextStateID)
{
	if(StateAnimation == nullptr)
	{
		return;
	}

	Character->StopAnimMontage(StateAnimation);
}

void USmashCharacterState::StateTick(float DeltaTime) {}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SmashCharacterState.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"

ESmashCharacterStateID USmashCharacterState::GetStateID()
{
	return ESmashCharacterStateID::None;
}

void USmashCharacterState::StateInit(USmashCharacterStateMachine* InStateMachine)
{
	StateMachine = InStateMachine;
	Character = InStateMachine->GetCharacter();
	CharacterSettings = GetDefault<USmashCharacterSettings>();
}

void USmashCharacterState::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	PlayStateAnimation();
}

void USmashCharacterState::StateExit(ESmashCharacterStateID NextStateID) {}

void USmashCharacterState::StateTick(float DeltaTime) {}

void USmashCharacterState::PlayStateAnimation()
{
	if(StateAnimation == nullptr)
	{
		Character->StopAnimMontage(nullptr);
		return;
	}

	Character->PlayAnimMontage(StateAnimation);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SmashCharacterState.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"

USmashCharacterState::USmashCharacterState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USmashCharacterState::BeginPlay()
{
	Super::BeginPlay();

	CharacterSettings = GetDefault<USmashCharacterSettings>();
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
		Character->StopAnimMontage(nullptr);
		return;
	}

	Character->PlayAnimMontage(StateAnimation);
}

void USmashCharacterState::StateExit(ESmashCharacterStateID NextStateID) {}

void USmashCharacterState::StateTick(float DeltaTime) {}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterStateMachine.h"
#include "SmashCharacterState.h"
#include "Characters/SmashCharacter.h"

void USmashCharacterStateMachine::Init(ASmashCharacter* InCharacter)
{
	Character = InCharacter;
	FindStates();
	InitStates();

	ChangeState(ESmashCharacterStateID::Idle);
}

void USmashCharacterStateMachine::Tick(float DeltaTime)
{
	if(CurrentState == nullptr)
	{
		return;
	}

	CurrentState->StateTick(DeltaTime);
}

ASmashCharacter* USmashCharacterStateMachine::GetCharacter() const
{
	return Character;
}

void USmashCharacterStateMachine::ChangeState(ESmashCharacterStateID NextStateID)
{
	USmashCharacterState* NextState = GetState(NextStateID);

	if(NextState == nullptr)
	{
		return;
	}

	if(CurrentState != nullptr)
	{
		CurrentState->StateExit(NextStateID);
	}

	ESmashCharacterStateID PreviousStateID = CurrentStateID;
	CurrentStateID = NextStateID;
	CurrentState = NextState;

	if(CurrentState == nullptr)
	{
		return;
	}

	CurrentState->StateEnter(PreviousStateID);
}

USmashCharacterState* USmashCharacterStateMachine::GetState(ESmashCharacterStateID StateID)
{
	for (USmashCharacterState* State : AllStates)
	{
		if(StateID == State->GetStateID())
		{
			return State;
		}
	}

	return nullptr;
}

void USmashCharacterStateMachine::FindStates()
{
	for (USmashCharacterState* State : Character->GetStates())
	{
		if(State == nullptr)
		{
			continue;
		}

		if(State->GetStateID() == ESmashCharacterStateID::None)
		{
			continue;
		}

		AllStates.Add(State);
	}
}

void USmashCharacterStateMachine::InitStates()
{
	for (USmashCharacterState* State : AllStates)
	{
		State->StateInit(this);
	}
}

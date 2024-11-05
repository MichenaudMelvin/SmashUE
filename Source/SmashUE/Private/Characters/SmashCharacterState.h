// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterStateID.h"
#include "Components/ActorComponent.h"
#include "SmashCharacterState.generated.h"


class USmashCharacterStateMachine;
class ASmashCharacter;

UCLASS(Abstract)
class SMASHUE_API USmashCharacterState : public UActorComponent
{
	GENERATED_BODY()

public:
	USmashCharacterState();

	virtual ESmashCharacterStateID GetStateID();

	virtual void StateInit(USmashCharacterStateMachine* InStateMachine);

protected:
	UPROPERTY()
	TObjectPtr<ASmashCharacter> Character;

	UPROPERTY()
	TObjectPtr<USmashCharacterStateMachine> StateMachine;
};

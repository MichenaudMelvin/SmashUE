// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterStateID.h"
#include "Components/ActorComponent.h"
#include "SmashCharacterState.generated.h"

class USmashCharacterSettings;
class USmashCharacterStateMachine;
class ASmashCharacter;

UCLASS(Abstract, Blueprintable)
class SMASHUE_API USmashCharacterState : public UObject
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID();

	virtual void StateInit(USmashCharacterStateMachine* InStateMachine);

	virtual void StateEnter(ESmashCharacterStateID PreviousStateID);

	virtual void StateExit(ESmashCharacterStateID  NextStateID);

	virtual void StateTick(float DeltaTime);

	virtual void PlayStateAnimation();

protected:
	UPROPERTY()
	TObjectPtr<ASmashCharacter> Character;

	UPROPERTY()
	TObjectPtr<USmashCharacterStateMachine> StateMachine;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> StateAnimation;

	UPROPERTY()
	const USmashCharacterSettings* CharacterSettings;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "Debug", meta = (ToolTip = "Enable debug features for the current state"))
	bool bDebugState = false;
#endif
};
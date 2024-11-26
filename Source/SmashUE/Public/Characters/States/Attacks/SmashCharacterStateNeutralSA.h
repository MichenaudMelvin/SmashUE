// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "SmashCharacterStateNeutralSA.generated.h"

UCLASS()
class SMASHUE_API USmashCharacterStateNeutralSA : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Attack", meta = (ClampMin = 0.0f))
	float Damages = 1.0f;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmashCharacterStateID.h"
#include "Engine/DeveloperSettings.h"
#include "SmashCharacterSettings.generated.h"

class ASmashCharacter;
class USmashCharacterState;
class UInputMappingContext;
class USmashCharacterInputData;

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Smash Character Settings"))
class SMASHUE_API USmashCharacterSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Inputs")
	TSoftObjectPtr<USmashCharacterInputData> InputData;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Inputs", meta = (ClampMin = 0.0f, ClampMax = 1.0f, UIMin = 0.0f, UIMax = 1.0f))
	float InputMoveXThreshold = 0.1f;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Inputs", meta = (ClampMin = 0.0f, ClampMax = 1.0f, UIMin = 0.0f, UIMax = 1.0f))
	float InputMoveYThreshold = 0.1f;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Orient", DisplayName = "2D Orient")
	bool b2DOrient = true;

	UPROPERTY(Config, EditDefaultsOnly, Category = "States", DisplayName = "Default Character States")
	TMap<ESmashCharacterStateID, TSubclassOf<USmashCharacterState>> CharacterStates;
};

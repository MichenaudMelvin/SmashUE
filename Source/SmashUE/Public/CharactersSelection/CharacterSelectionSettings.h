// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CharacterSelectionSettings.generated.h"

class ACharacterSelectionPawn;
class UCharacterSelectionInputData;

UCLASS(Config = Game, defaultconfig)
class SMASHUE_API UCharacterSelectionSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Inputs")
	TSoftObjectPtr<UCharacterSelectionInputData> InputData;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Characters")
	TSubclassOf<ACharacterSelectionPawn> CharacterSelectionClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Characters Selection", meta = (ClampMin = 1.0f))
	float CursorSpeed = 5.0f;
};

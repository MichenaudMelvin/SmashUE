// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ArenaSettings.generated.h"

class ASmashCharacter;

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Smash Arena Settings"))
class SMASHUE_API UArenaSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Characters")
	TSubclassOf<ASmashCharacter> SmashCharacterClassP0;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Characters")
	TSubclassOf<ASmashCharacter> SmashCharacterClassP1;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Characters")
	TSubclassOf<ASmashCharacter> SmashCharacterClassP2;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Characters")
	TSubclassOf<ASmashCharacter> SmashCharacterClassP3;

	//Temp
	UPROPERTY(Config, EditDefaultsOnly, Category = "TEMP", meta = (UIMin = 1, UIMax = 4, ClampMin = 1, ClampMax = 4, ToolTip = "TempVariable"))
	int NumbersOfPlayers = 4;
};

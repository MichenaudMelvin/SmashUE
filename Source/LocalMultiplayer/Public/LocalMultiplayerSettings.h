// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "LocalMultiplayerSettings.generated.h"

class UInputMappingContext;

UENUM()
enum class ELocalMultiplayerInputMappingType : uint8
{
	InGame,
	Menu
};

USTRUCT()
struct FLocalMultiplayerProfileData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Profile")
	FName ProfileName;

	UPROPERTY(EditAnywhere, Category = "Profile")
	TObjectPtr<UInputMappingContext> IMCInGame;

	UPROPERTY(EditAnywhere, Category = "Profile")
	TObjectPtr<UInputMappingContext> IMCMenu;

	bool ContainsKey(const FKey& Key, ELocalMultiplayerInputMappingType MappingType) const;

	UInputMappingContext* GetIMCFromType(ELocalMultiplayerInputMappingType MappingType) const;
};

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Local Multiplayer Settings"))
class LOCALMULTIPLAYER_API ULocalMultiplayerSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "LocalMultiplayer")
	TArray<FLocalMultiplayerProfileData> KeyboardProfilesData;

	int GetNBKeyboardProfiles() const;

	int FindKeyboardProfileIndexFromKey(const FKey& Key, ELocalMultiplayerInputMappingType MappingType) const;

	UPROPERTY(Config, EditDefaultsOnly, Category = "LocalMultiplayer")
	FLocalMultiplayerProfileData GamepadProfileData;

	UPROPERTY(Config, EditDefaultsOnly, Category = "LocalMultiplayer", meta = (ClampMin = 0, ClampMax = 4, UIMin = 0, UIMax = 4))
	int NBMaxGamepads = 4;
};

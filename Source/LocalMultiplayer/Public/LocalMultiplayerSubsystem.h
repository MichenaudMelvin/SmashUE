// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LocalMultiplayerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAssignNewPlayer, int, PlayerIndex);

enum class ELocalMultiplayerInputMappingType : uint8;

UCLASS()
class LOCALMULTIPLAYER_API ULocalMultiplayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "LocalMultiplayerSubsystem")
	void CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType);

	int GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex);

	int AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex);

	void AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex, ELocalMultiplayerInputMappingType MappingType) const;

	int GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID);

	int AssignNewPlayerToGamepadDeviceID(int DeviceID);

	void AssignGamepadInputMapping(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const;

	int GetCurrentPlayerNumber() const;

	UPROPERTY(BlueprintAssignable, Category = "LocalMultiplayer")
	FOnAssignNewPlayer OnAssignNewPlayer;

	static constexpr int MaxPlayerNumber = 4;

	static ELocalMultiplayerInputMappingType CurrentMappingType;

protected:
	UPROPERTY()
	uint8 LastAssignedPlayerIndex = 0;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromKeyboardProfileIndex;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromGamepadProfileIndex;
};

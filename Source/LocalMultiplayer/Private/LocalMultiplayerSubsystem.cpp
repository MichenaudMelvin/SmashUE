// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "LocalMultiplayerSettings.h"
#include "Kismet/GameplayStatics.h"

void ULocalMultiplayerSubsystem::CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType)
{
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
	int NBControllers = Settings->GetNBKeyboardProfiles() + Settings->NBMaxGamepads;

	for (int i = 0; i < Settings->GetNBKeyboardProfiles(); i++)
	{
		UGameplayStatics::CreatePlayer(this, i, true);
	}

	for (int i = Settings->GetNBKeyboardProfiles(); i < NBControllers; i++)
	{
		UGameplayStatics::CreatePlayer(this, i, true);
	}
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex)
{
	int* PlayerIndex = PlayerIndexFromKeyboardProfileIndex.Find(KeyboardProfileIndex);
	return PlayerIndex == nullptr ? INDEX_NONE : *PlayerIndex;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex)
{
	if(PlayerIndexFromKeyboardProfileIndex.Contains(KeyboardProfileIndex))
	{
		return INDEX_NONE;
	}

	int PlayerIndex = LastAssignedPlayerIndex;
	PlayerIndexFromKeyboardProfileIndex.Add(KeyboardProfileIndex, PlayerIndex);
	LastAssignedPlayerIndex++;
	return PlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex, ELocalMultiplayerInputMappingType MappingType) const
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, PlayerIndex);
	if(PlayerController == nullptr)
	{
		return;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if(LocalPlayer == nullptr)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if(Subsystem == nullptr)
	{
		return;
	}

	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
	UInputMappingContext* MappingContext = Settings->KeyboardProfilesData[KeyboardProfileIndex].GetIMCFromType(MappingType);

	FModifyContextOptions ContextOptions;
	ContextOptions.bForceImmediately = true;
	Subsystem->AddMappingContext(MappingContext, 0, ContextOptions);
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID)
{
	int* PlayerIndex = PlayerIndexFromGamepadProfileIndex.Find(DeviceID);
	return PlayerIndex == nullptr ? INDEX_NONE : *PlayerIndex;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID)
{
	if(PlayerIndexFromKeyboardProfileIndex.Contains(DeviceID))
	{
		return INDEX_NONE;
	}

	int PlayerIndex = LastAssignedPlayerIndex;
	PlayerIndexFromKeyboardProfileIndex.Add(DeviceID, PlayerIndex);
	LastAssignedPlayerIndex++;
	return PlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, PlayerIndex);
	if(PlayerController == nullptr)
	{
		return;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if(LocalPlayer == nullptr)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if(Subsystem == nullptr)
	{
		return;
	}

	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
	UInputMappingContext* MappingContext = Settings->GamepadProfileData.GetIMCFromType(MappingType);

	FModifyContextOptions ContextOptions;
	ContextOptions.bForceImmediately = true;
	Subsystem->AddMappingContext(MappingContext, 0, ContextOptions);
}

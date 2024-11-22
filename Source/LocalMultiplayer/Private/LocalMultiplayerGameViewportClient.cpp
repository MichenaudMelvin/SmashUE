// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerGameViewportClient.h"
#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerInput.h"

void ULocalMultiplayerGameViewportClient::PostInitProperties()
{
	Super::PostInitProperties();

	MaxSplitscreenPlayers = 8;
}

bool ULocalMultiplayerGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	ULocalMultiplayerSubsystem* Subsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();

	int KeyboardProfileIndex = Settings->FindKeyboardProfileIndexFromKey(EventArgs.Key, ELocalMultiplayerInputMappingType::InGame);

	if(KeyboardProfileIndex == INDEX_NONE)
	{
		return Super::InputKey(EventArgs);
	}

	int PlayerIndex = EventArgs.IsGamepad() ? Subsystem->GetAssignedPlayerIndexFromGamepadDeviceID(EventArgs.ControllerId)
											: Subsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);

	if(PlayerIndex == INDEX_NONE)
	{
		if(EventArgs.IsGamepad())
		{
			PlayerIndex = Subsystem->AssignNewPlayerToGamepadDeviceID(EventArgs.ControllerId);
			Subsystem->AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::InGame);
		}
		else
		{
			PlayerIndex = Subsystem->AssignNewPlayerToKeyboardProfile(KeyboardProfileIndex);
			Subsystem->AssignKeyboardMapping(PlayerIndex, KeyboardProfileIndex, ELocalMultiplayerInputMappingType::InGame);
		}
	}

	if(PlayerIndex == INDEX_NONE)
	{
		return Super::InputKey(EventArgs);
	}

	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, PlayerIndex);

	FInputKeyParams InputKeyParams(EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed, EventArgs.IsGamepad(), EventArgs.InputDevice);
	return Controller->InputKey(InputKeyParams);
}

bool ULocalMultiplayerGameViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId InputDevice, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	ULocalMultiplayerSubsystem* Subsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();

	int KeyboardProfileIndex = Settings->FindKeyboardProfileIndexFromKey(Key, ELocalMultiplayerInputMappingType::InGame);

	if(KeyboardProfileIndex == INDEX_NONE)
	{
		return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
	}

	bGamepad ? Subsystem->AssignNewPlayerToGamepadDeviceID(InputDevice.GetId())
			 : Subsystem->AssignNewPlayerToKeyboardProfile(KeyboardProfileIndex);

	int PlayerIndex = bGamepad ? Subsystem->GetAssignedPlayerIndexFromGamepadDeviceID(InputDevice.GetId())
							   : Subsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);

	if(PlayerIndex == INDEX_NONE)
	{
		if(bGamepad)
		{
			PlayerIndex = Subsystem->AssignNewPlayerToGamepadDeviceID(InputDevice.GetId());
			Subsystem->AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::InGame);
		}
		else
		{
			PlayerIndex = Subsystem->AssignNewPlayerToKeyboardProfile(KeyboardProfileIndex);
			Subsystem->AssignKeyboardMapping(PlayerIndex, KeyboardProfileIndex, ELocalMultiplayerInputMappingType::InGame);
		}
	}

	if(PlayerIndex == INDEX_NONE)
	{
		return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
	}

	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, PlayerIndex);

	FInputKeyParams InputKeyParams(Key, 0, DeltaTime, NumSamples, bGamepad, InputDevice);
	return Controller->InputKey(InputKeyParams);
}

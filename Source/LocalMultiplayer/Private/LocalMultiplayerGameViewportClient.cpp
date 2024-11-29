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

	int PlayerIndex = INDEX_NONE;

	if(EventArgs.IsGamepad())
	{
		PlayerIndex = Subsystem->GetAssignedPlayerIndexFromGamepadDeviceID(EventArgs.ControllerId);

		if(PlayerIndex == INDEX_NONE)
		{
			PlayerIndex = Subsystem->AssignNewPlayerToGamepadDeviceID(EventArgs.ControllerId);
			Subsystem->AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::Menu);
		}
	}
	else
	{
		int KeyboardProfileIndex = Settings->FindKeyboardProfileIndexFromKey(EventArgs.Key, ELocalMultiplayerInputMappingType::Menu);
		if(KeyboardProfileIndex == INDEX_NONE)
		{
			return Super::InputKey(EventArgs);
		}

		PlayerIndex = Subsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);

		if(PlayerIndex == INDEX_NONE)
		{
			PlayerIndex = Subsystem->AssignNewPlayerToKeyboardProfile(KeyboardProfileIndex);
			Subsystem->AssignKeyboardMapping(PlayerIndex, KeyboardProfileIndex, ELocalMultiplayerInputMappingType::Menu);
		}
	}

	if(PlayerIndex == INDEX_NONE)
	{
		return Super::InputKey(EventArgs);
	}

	Super::InputKey(EventArgs); // still calling it for editor features (such as console commands)

	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, PlayerIndex);

	FInputKeyParams InputKeyParams(EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed, EventArgs.IsGamepad(), EventArgs.InputDevice);
	return Controller->InputKey(InputKeyParams);
}

bool ULocalMultiplayerGameViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId InputDevice, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	ULocalMultiplayerSubsystem* Subsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();

	int PlayerIndex = INDEX_NONE;

	if(bGamepad)
	{
		PlayerIndex = Subsystem->GetAssignedPlayerIndexFromGamepadDeviceID(InputDevice.GetId());

		if(PlayerIndex == INDEX_NONE)
		{
			PlayerIndex = Subsystem->AssignNewPlayerToGamepadDeviceID(InputDevice.GetId());
			Subsystem->AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::Menu);
		}
	}
	else
	{
		int KeyboardProfileIndex = Settings->FindKeyboardProfileIndexFromKey(Key, ELocalMultiplayerInputMappingType::Menu);
		if(KeyboardProfileIndex == INDEX_NONE)
		{
			return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
		}

		PlayerIndex = Subsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);

		if(PlayerIndex == INDEX_NONE)
		{
			PlayerIndex = Subsystem->AssignNewPlayerToKeyboardProfile(KeyboardProfileIndex);
			Subsystem->AssignKeyboardMapping(PlayerIndex, KeyboardProfileIndex, ELocalMultiplayerInputMappingType::Menu);
		}
	}

	if(PlayerIndex == INDEX_NONE)
	{
		return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
	}

	Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad); // still calling it for editor features (such as console commands)

	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, PlayerIndex);

	FInputKeyParams InputKeyParams(Key, Delta, DeltaTime, NumSamples, bGamepad, InputDevice);
	return Controller->InputKey(InputKeyParams);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSettings.h"
#include "InputMappingContext.h"

bool FLocalMultiplayerProfileData::ContainsKey(const FKey& Key, ELocalMultiplayerInputMappingType MappingType) const
{
	UInputMappingContext* MappingContext = GetIMCFromType(MappingType);

	if (MappingContext == nullptr)
	{
		return false;
	}

	TArray<FEnhancedActionKeyMapping> Mappings = MappingContext->GetMappings();
	for (const FEnhancedActionKeyMapping& KeyMap : Mappings)
	{
		if(KeyMap.Key == Key)
		{
			return true;
		}
	}

	return false;
}

UInputMappingContext* FLocalMultiplayerProfileData::GetIMCFromType(ELocalMultiplayerInputMappingType MappingType) const
{
	switch (MappingType)
	{
	case ELocalMultiplayerInputMappingType::InGame:
		return IMCInGame;

	case ELocalMultiplayerInputMappingType::Menu:
		return IMCMenu;
	}

	return nullptr;
}

int ULocalMultiplayerSettings::GetNBKeyboardProfiles() const
{
	return KeyboardProfilesData.Num();
}

int ULocalMultiplayerSettings::FindKeyboardProfileIndexFromKey(const FKey& Key, ELocalMultiplayerInputMappingType MappingType) const
{
	for (int i = 0; i < KeyboardProfilesData.Num(); i++)
	{
		if(KeyboardProfilesData[i].ContainsKey(Key, MappingType))
		{
			return i;
		}
	}

	return INDEX_NONE;
}

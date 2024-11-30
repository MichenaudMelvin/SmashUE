// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UICharacterGridCell.h"

void UUICharacterGridCell::NativePreConstruct()
{
	Super::NativePreConstruct();

	InitCharacterAvatar();
}

void UUICharacterGridCell::SetCharacterSelected(bool bSelect, int PlayerIndex)
{
	bIsSelected = bSelect;
	bIsSelected ? OnCharacterSelected.Broadcast(PlayerIndex, CharacterID) : OnCharacterUnselected.Broadcast(PlayerIndex);
}

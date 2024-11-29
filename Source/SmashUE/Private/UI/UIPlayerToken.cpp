// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIPlayerToken.h"
#include "UI/UICharacterSelection.h"

bool UUIPlayerToken::CanDropToken() const
{
	if (CharacterSelection == nullptr)
	{
		return false;
	}

	return CharacterSelection->DoesTokenOverlapACharacterIcon(this);
}

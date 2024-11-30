// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIPlayerToken.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"
#include "UI/UICharacterGridCell.h"
#include "UI/UICharacterSelection.h"
#include "UI/UIPlayerCursor.h"

void UUIPlayerToken::GrabToken()
{
	if (CharacterSelection == nullptr)
	{
		return;
	}

	UUICharacterGridCell* CharacterGridCell = CharacterSelection->GetOverlappedCharacterIcon(this);
	if (CharacterGridCell == nullptr)
	{
		return;
	}

	CharacterGridCell->SetCharacterSelected(false, PlayerNumber);
}

void UUIPlayerToken::TryToDropToken()
{
	if (CharacterSelection == nullptr || PlayerCursor == nullptr)
	{
		return;
	}

	UUICharacterGridCell* CharacterGridCell = CharacterSelection->GetOverlappedCharacterIcon(this);
	if (CharacterGridCell == nullptr || CharacterGridCell->IsCharacterSelected())
	{
		return;
	}

	CharacterGridCell->SetCharacterSelected(true, PlayerNumber);
	PlayerCursor->DetachToken();

	UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Slot);
	if (CanvasPanelSlot == nullptr)
	{
		return;
	}

	FVector2D ParentPos = CharacterGridCell->GetParent()->GetPaintSpaceGeometry().GetLocalPositionAtCoordinates(FVector2D::ZeroVector);
	FVector2D ChildPos = CharacterGridCell->GetPaintSpaceGeometry().GetLocalPositionAtCoordinates(FVector2f(0.25f));
	FVector2D TargetTokenPosition = ParentPos + ChildPos;
	CanvasPanelSlot->SetPosition(TargetTokenPosition);
}

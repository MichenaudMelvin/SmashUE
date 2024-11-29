// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIPlayerCursor.h"

#include "UsefulFunctions.h"
#include "CharactersSelection/CharacterSelectionSettings.h"
#include "CharactersSelection/CharacterSelectionPawn.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/UIPlayerToken.h"

void UUIPlayerCursor::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetCursorState(Opened);
}

void UUIPlayerCursor::TickCursor(const FGeometry& ParentGeometry, float DeltaTime)
{
	Move(ParentGeometry, DeltaTime);

	switch (CursorState)
	{
	case None:
		return;
	case Opened:
		OnOpened(ParentGeometry, DeltaTime);
		break;
	case Pinched:
		OnPinched(ParentGeometry, DeltaTime);
		break;
	case Point:
		OnPoint(ParentGeometry, DeltaTime);
		break;
	}
}

void UUIPlayerCursor::SetCursorState_Implementation(ECursorState NewState)
{
	CursorState = NewState;
}

void UUIPlayerCursor::AttachToken()
{
	SetCursorState(Pinched);
}

void UUIPlayerCursor::DetachToken()
{
	SetCursorState(Opened);
}

void UUIPlayerCursor::Move(const FGeometry& ParentGeometry, float DeltaTime)
{
	const UCharacterSelectionSettings* Settings = GetDefault<UCharacterSelectionSettings>();
	UCanvasPanelSlot* CurrentSlot = Cast<UCanvasPanelSlot>(Slot);

	if (CurrentPawn == nullptr || CurrentSlot == nullptr)
	{
		return;
	}

	FVector2D TargetPosition = CurrentSlot->GetPosition() + (CurrentPawn->GetInputMove() * Settings->CursorSpeed * DeltaTime);

	// TODO add size of cursor
	TargetPosition.X = FMath::Clamp(TargetPosition.X, 0, ParentGeometry.Size.X);
	TargetPosition.Y = FMath::Clamp(TargetPosition.Y, 0, ParentGeometry.Size.Y);

	CurrentSlot->SetPosition(TargetPosition);

	if (CurrentToken == nullptr || CursorState != Pinched)
	{
		return;
	}

	UCanvasPanelSlot* TokenSlot = Cast<UCanvasPanelSlot>(CurrentToken->Slot);
	if (TokenSlot == nullptr)
	{
		return;
	}

	TokenSlot->SetPosition(TargetPosition);
}

void UUIPlayerCursor::OnOpened(const FGeometry& ParentGeometry, float DeltaTime)
{
	if (CurrentPawn == nullptr)
	{
		return;
	}

	if (!CurrentPawn->GetInteractInput())
	{
		bReleasedInteraction = true;
	}

	else if (CurrentPawn->GetInteractInput() && bReleasedInteraction)
	{
		bool bOverlap = UUsefulFunctions::DoesWidgetsOverlaps(this, CurrentToken);

		if (bOverlap)
		{
			AttachToken();
			bReleasedInteraction = false;
		}
	}
}

void UUIPlayerCursor::OnPinched(const FGeometry& ParentGeometry, float DeltaTime)
{
	if(CurrentPawn == nullptr || CurrentToken == nullptr)
	{
		return;
	}

	if (!CurrentPawn->GetInteractInput())
	{
		bReleasedInteraction = true;
	}

	else if(CurrentPawn->GetInteractInput() && bReleasedInteraction)
	{
		if (CurrentToken->CanDropToken())
		{
			DetachToken();
			bReleasedInteraction = false;
		}
	}
}

void UUIPlayerCursor::OnPoint(const FGeometry& ParentGeometry, float DeltaTime)
{
	
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIPlayerCursor.h"

#include "UsefulFunctions.h"
#include "CharactersSelection/CharacterSelectionSettings.h"
#include "CharactersSelection/CharacterSelectionPawn.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"
#include "UI/UIPlayerToken.h"
#include "UI/UIStockButton.h"

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
	bReleasedInteraction = false;
}

void UUIPlayerCursor::AttachToken()
{
	CurrentToken->GrabToken();
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

	if (CurrentPawn == nullptr || CurrentSlot == nullptr || GetParent() == nullptr)
	{
		return;
	}

	FVector2D TargetPosition = CurrentSlot->GetPosition() + (CurrentPawn->GetInputMove() * Settings->CursorSpeed * DeltaTime);

	FVector2D CursorSize = GetPaintSpaceGeometry().GetAbsoluteSize();
	FVector2D ParentSize = GetParent()->GetPaintSpaceGeometry().GetLocalSize();

	if (ParentSize == FVector2D::ZeroVector || CursorSize == FVector2D::ZeroVector)
	{
		return;
	}

	TargetPosition.X = FMath::Clamp(TargetPosition.X, 0, ParentSize.X - (CursorSize.X * 2));
	TargetPosition.Y = FMath::Clamp(TargetPosition.Y, 0, ParentSize.Y - (CursorSize.Y * 2));

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
		CurrentToken->TryToDropToken();
	}
}

void UUIPlayerCursor::OnPoint(const FGeometry& ParentGeometry, float DeltaTime)
{
	if(CurrentPawn == nullptr)
	{
		return;
	}

	if (!CurrentPawn->GetInteractInput())
	{
		bReleasedInteraction = true;
	}

	if (CurrentPawn->GetInteractInput() && bReleasedInteraction)
	{
		UUIStockButton* StockButton = Cast<UUIStockButton>(OverlappedButton);
		if (StockButton == nullptr)
		{
			return;
		}

		StockButton->Click();
		bReleasedInteraction = false;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UICharacterSelection.h"
#include "LocalMultiplayerSubsystem.h"
#include "SmashGameInstance.h"
#include "UsefulFunctions.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "CharactersSelection/CharacterSelectionPawn.h"
#include "CharactersSelection/CharactersSelectionGameMode.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UICharacterGridCell.h"
#include "UI/UIPlayerCursor.h"
#include "UI/UIPlayerToken.h"
#include "UI/UIPreviewPanel.h"
#include "UI/UIStockButton.h"

void UUICharacterSelection::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Cursors.Add(CursorP1);
	Cursors.Add(CursorP2);
	Cursors.Add(CursorP3);
	Cursors.Add(CursorP4);

	Tokens.Add(TokenP1);
	Tokens.Add(TokenP2);
	Tokens.Add(TokenP3);
	Tokens.Add(TokenP4);

	ULocalMultiplayerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<ULocalMultiplayerSubsystem>();
	Subsystem->OnAssignNewPlayer.AddDynamic(this, &UUICharacterSelection::AddPlayer);

	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, Widgets, UUIStockButton::StaticClass(), false);

	for (UUserWidget* Widget : Widgets)
	{
		UUIStockButton* StockButton = Cast<UUIStockButton>(Widget);
		if (StockButton == nullptr)
		{
			continue;
		}

		StockButtons.Add(StockButton);
	}

	TArray<AActor*> OutPawns;
	UGameplayStatics::GetAllActorsOfClass(this, ACharacterSelectionPawn::StaticClass(), OutPawns);
	for (int i = 0; i < OutPawns.Num(); ++i)
	{
		ACharacterSelectionPawn* CurrentPawn = Cast<ACharacterSelectionPawn>(OutPawns[i]);
		Pawns.Add(CurrentPawn);

		Cursors[i]->InitCursor(CurrentPawn, Tokens[i], i);
		Tokens[i]->InitToken(i, Cursors[i], this);
	}

	if (CharactersIconPanel == nullptr)
	{
		return;
	}

	for (UWidget* Widget : CharactersIconPanel->GetAllChildren())
	{
		UUICharacterGridCell* CharacterGridCell = Cast<UUICharacterGridCell>(Widget);
		if (CharacterGridCell == nullptr)
		{
			continue;
		}

		CharactersIcons.Add(CharacterGridCell);
		CharacterGridCell->OnCharacterSelected.AddDynamic(this, &UUICharacterSelection::EnablePanel);
		CharacterGridCell->OnCharacterUnselected.AddDynamic(this, &UUICharacterSelection::DisablePanel);
	}

	if (PreviewPanelContainer == nullptr)
	{
		return;
	}

	for (UWidget* Widget : PreviewPanelContainer->GetAllChildren())
	{
		UUIPreviewPanel* PreviewPanel = Cast<UUIPreviewPanel>(Widget);
		if (PreviewPanel == nullptr)
		{
			continue;
		}

		PreviewPanels.Add(PreviewPanel);
		PreviewPanel->OnPanelToggleState.AddDynamic(this, &UUICharacterSelection::SetReadyPanelVisibility);
	}
}

void UUICharacterSelection::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CursorTick(MyGeometry, InDeltaTime);
	CheckCharactersReadyState();
}

void UUICharacterSelection::AddPlayer(int ElementsIndex)
{
	if (ElementsIndex >= ULocalMultiplayerSubsystem::MaxPlayerNumber)
	{
		return;
	}

	Cursors[ElementsIndex]->SetVisibility(ESlateVisibility::Visible);
	Tokens[ElementsIndex]->SetVisibility(ESlateVisibility::Visible);
}

void UUICharacterSelection::CursorTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	for (UUIPlayerCursor* CurrentCursor : Cursors)
	{
		CurrentCursor->TickCursor(MyGeometry, InDeltaTime);
		if (CurrentCursor->GetCursorState() == Pinched)
		{
			continue;
		}

		UUIStockButton* TargetStockButton = nullptr;
		for (UUIStockButton* StockButton : StockButtons)
		{
			if (UUsefulFunctions::DoesWidgetsOverlaps(CurrentCursor, StockButton))
			{
				TargetStockButton = StockButton;
				break;
			}
		}

		ECursorState NewCursorState = TargetStockButton != nullptr ? Point : Opened;
		if (NewCursorState == CurrentCursor->GetCursorState())
		{
			continue;
		}

		CurrentCursor->SetCursorState(NewCursorState);
		CurrentCursor->SetOverlappedButton(TargetStockButton);
	}
}

void UUICharacterSelection::CheckCharactersReadyState()
{
	ULocalMultiplayerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<ULocalMultiplayerSubsystem>();

	if (Subsystem == nullptr || Subsystem->GetCurrentPlayerNumber() < 2)
	{
		return;
	}

	for (int i = 0; i < Subsystem->GetCurrentPlayerNumber(); ++i)
	{
		ACharacterSelectionPawn* CurrentPawn = Pawns[i];
		UUIPreviewPanel* CurrentPanel = PreviewPanels[i];

		if (CurrentPawn == nullptr || CurrentPanel == nullptr)
		{
			continue;
		}

		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow,FString::Printf(TEXT("%d: %s"), i, (CurrentPawn->IsReady() ? TEXT("Ready") : TEXT("Waiting"))));

		if (!CurrentPanel->IsPanelEnabled() || !CurrentPawn->IsReady())
		{
			return;
		}
	}

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	UGameInstance* GameInstance = GetGameInstance();
	if (GameMode == nullptr || GameInstance == nullptr)
	{
		return;
	}

	ACharactersSelectionGameMode* CurrentGameMode = Cast<ACharactersSelectionGameMode>(GameMode);
	USmashGameInstance* SmashGameInstance = Cast<USmashGameInstance>(GetGameInstance());
	if (CurrentGameMode == nullptr || SmashGameInstance == nullptr)
	{
		return;
	}

	for (int i = 0; i < Subsystem->GetCurrentPlayerNumber(); ++i)
	{
		if (PreviewPanels[i] == nullptr)
		{
			continue;
		}

		SmashGameInstance->AddPlayerID(PreviewPanels[i]->GetCurrentCharacterID());
	}

	CurrentGameMode->LaunchGame();
}

UUICharacterGridCell* UUICharacterSelection::GetOverlappedCharacterIcon(const UUIPlayerToken* Token) const
{
	for (UUICharacterGridCell* CharacterIcon : CharactersIcons)
	{
		if (UUsefulFunctions::DoesWidgetsOverlaps(Token, CharacterIcon))
		{
			return CharacterIcon;
		}
	}

	return nullptr;
}

void UUICharacterSelection::EnablePanel(int PlayerIndex, const FName& CharacterID)
{
	if (!PreviewPanels.IsValidIndex(PlayerIndex))
	{
		return;
	}

	PreviewPanels[PlayerIndex]->UpdatePreviewPanel(CharacterID);
}

void UUICharacterSelection::DisablePanel(int PlayerIndex)
{
	if (!PreviewPanels.IsValidIndex(PlayerIndex))
	{
		return;
	}

	PreviewPanels[PlayerIndex]->UpdatePreviewPanel(NAME_None);
}

void UUICharacterSelection::SetReadyPanelVisibility()
{
	ULocalMultiplayerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<ULocalMultiplayerSubsystem>();

	if (Subsystem == nullptr || Subsystem->GetCurrentPlayerNumber() < 2)
	{
		return;
	}

	for (int i = 0; i < Subsystem->GetCurrentPlayerNumber(); ++i)
	{
		const UUIPreviewPanel* PreviewPanel = PreviewPanels[i];
		if (PreviewPanel == nullptr)
		{
			continue;
		}

		if (!PreviewPanel->IsPanelEnabled())
		{
			ReadyPanel->SetVisibility(ESlateVisibility::Hidden);
			return;
		}
	}

	ReadyPanel->SetVisibility(ESlateVisibility::Visible);
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIPlayerToken.h"
#include "Blueprint/UserWidget.h"
#include "UICharacterSelection.generated.h"

class UUIStockButton;
class UUIPreviewPanel;
class UUICharacterGridCell;
class UUIPlayerToken;
class UUIPlayerCursor;
class UCanvasPanelSlot;
class ACharacterSelectionPawn;

UCLASS()
class SMASHUE_API UUICharacterSelection : public UUserWidget
{
	GENERATED_BODY()

#pragma region Defaults

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void AddPlayer(int ElementsIndex);

#pragma endregion

#pragma region Cursors

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Cursors", meta = (BindWidget))
	TObjectPtr<UUIPlayerCursor> CursorP1;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Cursors", meta = (BindWidget))
	TObjectPtr<UUIPlayerCursor> CursorP2;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Cursors", meta = (BindWidget))
	TObjectPtr<UUIPlayerCursor> CursorP3;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Cursors", meta = (BindWidget))
	TObjectPtr<UUIPlayerCursor> CursorP4;

	UPROPERTY()
	TArray<UUIPlayerCursor*> Cursors;

	UPROPERTY()
	TArray<UUIStockButton*> StockButtons;

	void CursorTick(const FGeometry& MyGeometry, float InDeltaTime);

#pragma endregion

#pragma region Tokens

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Tokens", meta = (BindWidget))
	TObjectPtr<UUIPlayerToken> TokenP1;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Tokens", meta = (BindWidget))
	TObjectPtr<UUIPlayerToken> TokenP2;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Tokens", meta = (BindWidget))
	TObjectPtr<UUIPlayerToken> TokenP3;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Tokens", meta = (BindWidget))
	TObjectPtr<UUIPlayerToken> TokenP4;

	UPROPERTY()
	TArray<UUIPlayerToken*> Tokens;

#pragma endregion

#pragma region Characters

protected:
	UPROPERTY()
	TArray<ACharacterSelectionPawn*> Pawns;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Characters", meta = (BindWidget))
	TObjectPtr<UPanelWidget> CharactersIconPanel;

	UPROPERTY()
	TArray<UUICharacterGridCell*> CharactersIcons;

	void CheckCharactersReadyState();

public:
	UUICharacterGridCell* GetOverlappedCharacterIcon(const UUIPlayerToken* Token) const;
#pragma endregion

#pragma region PreviewPanels

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "PreviewPanel", meta = (BindWidget))
	TObjectPtr<UPanelWidget> PreviewPanelContainer;

	UPROPERTY()
	TArray<UUIPreviewPanel*> PreviewPanels;

	UFUNCTION()
	void EnablePanel(int PlayerIndex, const FName& CharacterID);

	UFUNCTION()
	void DisablePanel(int PlayerIndex);

#pragma endregion

#pragma region ReadyPanel

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Cursors", meta = (BindWidget))
	TObjectPtr<UPanelWidget> ReadyPanel;

	UFUNCTION()
	void UpdateReadyPanelVisibility();

#pragma endregion
};

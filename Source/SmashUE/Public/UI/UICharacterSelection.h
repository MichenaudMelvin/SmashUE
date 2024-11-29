// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIPlayerToken.h"
#include "Blueprint/UserWidget.h"
#include "UICharacterSelection.generated.h"

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
	UPROPERTY()
	TArray<ACharacterSelectionPawn*> Pawns;

	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Characters", meta = (BindWidget))
	TObjectPtr<UPanelWidget> CharactersIconPanel;

	UPROPERTY()
	TArray<UWidget*> CharactersIcons;

public:
	bool DoesTokenOverlapACharacterIcon(const UUIPlayerToken* Token) const;

#pragma endregion 
};

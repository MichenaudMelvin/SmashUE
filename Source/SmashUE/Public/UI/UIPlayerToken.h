// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIPlayerToken.generated.h"

class UUIPlayerCursor;
class UUICharacterSelection;

UCLASS()
class SMASHUE_API UUIPlayerToken : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Cursor")
	TObjectPtr<UUIPlayerCursor> PlayerCursor;

	UPROPERTY(BlueprintReadWrite, Category = "Token")
	TObjectPtr<UUICharacterSelection> CharacterSelection;

	UPROPERTY(BlueprintReadWrite, Category = "Token")
	int PlayerNumber = -1;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Display")
	void InitToken(int InPlayerNumber, UUIPlayerCursor* InCursor, UUICharacterSelection* InCharacterSelection);

	void GrabToken();

	void TryToDropToken();
};

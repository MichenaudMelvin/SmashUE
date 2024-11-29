// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIPlayerToken.generated.h"

class UUICharacterSelection;

UCLASS()
class SMASHUE_API UUIPlayerToken : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Display", meta = (BindWidget))
	TObjectPtr<UWidget> DetectionArea;

	UPROPERTY(BlueprintReadWrite, Category = "Token")
	TObjectPtr<UUICharacterSelection> CharacterSelection;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Display")
	void InitToken(int PlayerNumber, UUICharacterSelection* InCharacterSelection);

	const UWidget* GetDetectionArea() const {return DetectionArea;}

	bool CanDropToken() const;
};

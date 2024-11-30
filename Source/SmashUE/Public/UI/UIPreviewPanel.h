// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIPreviewPanel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPanelToggleState);

UCLASS()
class SMASHUE_API UUIPreviewPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Preview", meta = (ForceAsFunction))
	void Enable();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Preview", meta = (ForceAsFunction))
	void Disable();

	UPROPERTY(BlueprintReadWrite, Category = "Preview")
	bool bIsPanelEnabled = false;

	UPROPERTY(BlueprintReadWrite, Category = "Character")
	FName CurrentCharacterID;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Preview", meta = (ForceAsFunction))
	void UpdatePreviewPanel(const FName& CharacterID);

	bool IsPanelEnabled() const {return bIsPanelEnabled;}

	const FName& GetCurrentCharacterID() const {return CurrentCharacterID;}

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Preview")
	FOnPanelToggleState OnPanelToggleState;
};

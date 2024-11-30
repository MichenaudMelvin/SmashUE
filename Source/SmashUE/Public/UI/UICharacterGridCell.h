// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UICharacterGridCell.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterSelected, int, PlayerIndex, const FName&, CharacterID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterUnselected, int, PlayerIndex);

UCLASS()
class SMASHUE_API UUICharacterGridCell : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterID")
	FName CharacterID;

	UPROPERTY(BlueprintReadOnly, Category = "CharacterSelection")
	bool bIsSelected = false;

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterSelection", meta = (ForceAsFunction))
	void InitCharacterAvatar();

public:
	UPROPERTY(BlueprintAssignable, Category = "CharacterSelection")
	FOnCharacterSelected OnCharacterSelected;

	UPROPERTY(BlueprintAssignable, Category = "CharacterSelection")
	FOnCharacterUnselected OnCharacterUnselected;

	bool IsCharacterSelected() const {return bIsSelected;}

	void SetCharacterSelected(bool bSelect, int PlayerIndex);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIStockButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClick);

UCLASS()
class SMASHUE_API UUIStockButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "StockButton")
	FOnClick OnClick;

	void Click();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UsefulFunctions.generated.h"

class UWidget;
enum class ETriggerEvent : uint8;
class UInputAction;

UCLASS()
class SMASHUE_API UUsefulFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// the bound func needs to be a UFUNCTION
	static void BindAction(UEnhancedInputComponent* EnhancedInputComponent, const UInputAction* Action, const TArray<ETriggerEvent>& TriggerEvents, UObject* Object, const FName& FuncName);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Widgets")
	static bool DoesWidgetsOverlaps(const UWidget* WidgetA, const UWidget* WidgetB);
};

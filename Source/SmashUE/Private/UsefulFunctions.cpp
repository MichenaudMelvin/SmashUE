// Fill out your copyright notice in the Description page of Project Settings.


#include "UsefulFunctions.h"
#include "EnhancedInputComponent.h"
#include "Components/Widget.h"

void UUsefulFunctions::BindAction(UEnhancedInputComponent* EnhancedInputComponent, const UInputAction* Action, const TArray<ETriggerEvent>& TriggerEvents, UObject* Object, const FName& FuncName)
{
	for (const ETriggerEvent& Event : TriggerEvents)
	{
		EnhancedInputComponent->BindAction(Action, Event, Object, FuncName);
	}
}

bool UUsefulFunctions::DoesWidgetsOverlaps(const UWidget* WidgetA, const UWidget* WidgetB)
{
	if (WidgetA == nullptr || WidgetB == nullptr)
	{
		return false;
	}

	FVector2D WidgetAPosition = WidgetA->GetPaintSpaceGeometry().GetAbsolutePosition();
	FVector2D WidgetAOtherPoint = WidgetAPosition + WidgetA->GetPaintSpaceGeometry().GetAbsoluteSize();

	FVector2D WidgetBPosition = WidgetB->GetPaintSpaceGeometry().GetAbsolutePosition();
	FVector2D WidgetBOtherPoint = WidgetBPosition + WidgetB->GetPaintSpaceGeometry().GetAbsoluteSize();

	bool bNoOverlap = WidgetAPosition.X > WidgetBOtherPoint.X || WidgetBPosition.X > WidgetAOtherPoint.X || WidgetAPosition.Y > WidgetBOtherPoint.Y || WidgetBPosition.Y > WidgetAOtherPoint.Y;
	return !bNoOverlap;
}

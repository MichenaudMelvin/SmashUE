// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIStockButton.h"

void UUIStockButton::Click()
{
	OnClick.Broadcast();
}

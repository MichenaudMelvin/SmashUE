// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIPlayerCursor.generated.h"

class UUIPlayerToken;
class ACharacterSelectionPawn;

UENUM(BlueprintType)
enum ECursorState
{
	None,
	Opened,
	Pinched,
	Point,
};

UCLASS()
class SMASHUE_API UUIPlayerCursor : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly, Category = "Cursor")
	TEnumAsByte<ECursorState> CursorState;

	UPROPERTY(BlueprintReadWrite, Category = "Token")
	TObjectPtr<UUIPlayerToken> CurrentToken;

	UPROPERTY(BlueprintReadWrite, Category = "Token")
	TObjectPtr<ACharacterSelectionPawn> CurrentPawn;

	UPROPERTY(VisibleDefaultsOnly, Category = "Cursor", meta = (BindWidget))
	TObjectPtr<UWidget> DetectionArea;

public:
	void TickCursor(const FGeometry& ParentGeometry, float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent, Category = "Display")
	void InitCursor(ACharacterSelectionPawn* TargetPawn, UUIPlayerToken* TargetToken, int PlayerNumber);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Cursor")
	void SetCursorState(ECursorState NewState);

	UFUNCTION(BlueprintCallable, Category = "Token")
	void AttachToken();

	UFUNCTION(BlueprintCallable, Category = "Token")
	void DetachToken();

protected:
	void Move(const FGeometry& ParentGeometry, float DeltaTime);

	void OnOpened(const FGeometry& ParentGeometry, float DeltaTime);

	void OnPinched(const FGeometry& ParentGeometry, float DeltaTime);

	void OnPoint(const FGeometry& ParentGeometry, float DeltaTime);

	bool bReleasedInteraction = true;
};

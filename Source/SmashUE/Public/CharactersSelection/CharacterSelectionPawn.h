// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CharacterSelectionPawn.generated.h"

struct FInputActionValue;
class UCharacterSelectionInputData;

UCLASS()
class SMASHUE_API ACharacterSelectionPawn : public APawn
{
	GENERATED_BODY()

public:
	ACharacterSelectionPawn();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UCharacterSelectionInputData> InputData;

	bool bInputInteract;

	FVector2D InputMove;

	bool bInputStart;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetInputData(UCharacterSelectionInputData* InInputData);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inputs")
	bool GetInteractInput() const {return bInputInteract;}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inputs")
	FVector2D GetInputMove() const {return InputMove;}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inputs")
	bool IsReady() const {return bInputStart;}

private:
	UFUNCTION()
	void OnInputInteract(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void OnInputMove(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void OnInputStart(const FInputActionValue& InputActionValue);
};

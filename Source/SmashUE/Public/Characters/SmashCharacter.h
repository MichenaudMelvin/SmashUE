// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "SmashCharacter.generated.h"

class USmashCharacterInputData;
class UInputMappingContext;
class USmashCharacterStateMachine;

UCLASS()
class SMASHUE_API ASmashCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region UnrealDefaults

public:
	ASmashCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma endregion

#pragma region Orient

public:
	float GetOrientX() const;

	UFUNCTION(BlueprintCallable) // temp
	void SetOrientX(float NewOrientX);

protected:
	UPROPERTY(BlueprintReadOnly)
	float OrientX = 1.0f;

	void RotateMeshUsingOrientX() const;

#pragma endregion

#pragma region StateMachine

public:
	void CreateStateMachine();

	void InitStateMachine();

	void TickStateMachine(float DeltaTime) const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USmashCharacterStateMachine> StateMachine;

#pragma endregion

#pragma region InputData / MappingContext

public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<USmashCharacterInputData> InputData;

protected:
	void SetupMappingContextIntoController() const;

#pragma endregion

#pragma region InputMoveX

public:
	float GetInputMoveX() const;

protected:
	UPROPERTY()
	float InputMoveX = 0.0f;

private:
	void BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);

	void OnInputMoveX(const FInputActionValue& InputActionValue);

#pragma endregion
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Camera/CameraFollowTarget.h"
#include "GameFramework/Character.h"
#include "SmashCharacter.generated.h"

enum class ETriggerEvent : uint8;
class UInputAction;
enum class ESmashCharacterStateID : uint8;
class USmashCharacterState;
class USmashCharacterInputData;
class UInputMappingContext;
class USmashCharacterStateMachine;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveXEvent, float, InputMoveX);

UCLASS()
class SMASHUE_API ASmashCharacter : public ACharacter, public ICameraFollowTarget
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

	const TArray<USmashCharacterState*>& GetStates() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "StateMachine")
	TObjectPtr<USmashCharacterStateMachine> StateMachine;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "States")
	TArray<USmashCharacterState*> States;

	UPROPERTY(EditDefaultsOnly, Category = "States")
	TMap<ESmashCharacterStateID, TSubclassOf<USmashCharacterState>> CharacterStates;

	void CreateStates();

#pragma endregion

#pragma region InputData / MappingContext

public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<USmashCharacterInputData> InputData;

#pragma endregion

#pragma region Inputs

public:
	float GetInputMoveX() const;

	float GetInputMoveY() const;

	bool GetInputJump() const;

	bool GetInputBasicAttack() const;

	bool GetInputSpecialAttack() const;

	UPROPERTY()
	FInputMoveXEvent InputMoveXFastEvent;

protected:
	UPROPERTY()
	float InputMoveX = 0.0f;

	UPROPERTY()
	float InputMoveY = 0.0f;

	UPROPERTY()
	bool bInputJump = false;

	UPROPERTY()
	bool bInputBasicAttack = false;

	UPROPERTY()
	bool bInputSpecialAttack = false;

private:
	void BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);

	// the bound func needs to be a UFUNCTION
	void BindAction(UEnhancedInputComponent* EnhancedInputComponent, const UInputAction* Action, const TArray<ETriggerEvent>& TriggerEvents, const FName& FuncName);

	UFUNCTION()
	void OnInputMoveX(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void OnInputMoveY(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void OnInputMoveXFast(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void OnInputJump(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void OnInputBasicAttack(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void OnInputSpecialAttack(const FInputActionValue& InputActionValue);

#pragma endregion

#pragma region CameraTarget

public:
	virtual FVector GetFollowPosition() override;

	virtual bool IsFollowable() override;

#pragma endregion
};

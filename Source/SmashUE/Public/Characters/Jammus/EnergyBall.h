// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnergyBall.generated.h"

UENUM(BlueprintType)
enum class EEnergyBallState : uint8
{
	None = 0,
	Charging,
	Charged,
	Throwing,
};

USTRUCT()
struct FEnergyBallData
{
	GENERATED_BODY()

	float MaxChargingDuration = 1.0f;

	float EnergyBallSpeed = 1.0f;

	int ThrowDirection = 0;
};

UCLASS()
class SMASHUE_API AEnergyBall : public AActor
{
	GENERATED_BODY()

public:
	AEnergyBall();

protected:
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnergyBall")
	TObjectPtr<UStaticMeshComponent> EnergyBallMesh;

	UPROPERTY(EditDefaultsOnly, Category = "EnergyBall")
	FFloatRange MeshScale = FFloatRange(0.2f, 1.0f);

	UPROPERTY(BlueprintReadOnly, Category = "EnergyBall")
	EEnergyBallState EnergyBallState = EEnergyBallState::None;

	FEnergyBallData CurrentData;

	float ChargingDuration = 0.0f;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "EnergyBall|Debug", DisplayName = "ChargingPercent", meta = (ClampMin = 0.0f, ClampMax = 1.0f, UIMin = 0.0f, UIMax = 1.0f))
	float DebugChargingPercent = 1.0f;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	void InitEnergyBall(const FEnergyBallData& EnergyBallData);

	void ChargeEnergy(float DeltaTime);

	void CompleteCharge();

	void Throw(float DeltaTime);

	EEnergyBallState GetEnergyBallState() const {return EnergyBallState;}

	void SetEnergyBallState(const EEnergyBallState& NewState);
};

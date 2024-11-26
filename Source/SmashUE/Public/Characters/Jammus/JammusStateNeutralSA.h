// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnergyBall.h"
#include "Characters/States/Attacks/SmashCharacterStateNeutralSA.h"
#include "JammusStateNeutralSA.generated.h"

UCLASS()
class SMASHUE_API UJammusStateNeutralSA : public USmashCharacterStateNeutralSA
{
	GENERATED_BODY()

public:
	virtual void StateInit(USmashCharacterStateMachine* InStateMachine) override;

	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	virtual void StateExit(ESmashCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "EnergyBall")
	TSubclassOf<AEnergyBall> EnergyBallClass;

	UPROPERTY(EditDefaultsOnly, Category = "EnergyBall")
	FName SocketName = "Gun";

	UPROPERTY(EditDefaultsOnly, Category = "EnergyBall", meta = (ClampMin = 0.0f, Units = "s"))
	float ChargingDuration = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category = "EnergyBall", meta = (ClampMin = 0.0f, ForceUnits = "cm/s"))
	float EnergyBallSpeed = 10.0f;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "Attack|Debug", DisplayName = "ChargingPercent", meta = (ClampMin = 0.0f, ClampMax = 1.0f, UIMin = 0.0f, UIMax = 1.0f))
	float DebugChargingPercent = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, Category = "Attack|Debug", DisplayName = "DamagesAtChargingPercent")
	float DebugDamagesAtChargingPercent;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	UPROPERTY()
	TObjectPtr<AEnergyBall> CurrentEnergyBall;

	UPROPERTY()
	FEnergyBallData TargetEnergyBallData;

	UPROPERTY()
	UPrimitiveComponent* EnergyBallComponent;
};

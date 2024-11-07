// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "SmashCharacterStateFall.generated.h"


UCLASS(ClassGroup = (SmashCharacterState), meta = (BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateFall : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;

	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	virtual void StateExit(ESmashCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Fall", meta = (ClampMin = 0.0f))
	float FallHorizontalMoveSpeed = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Fall", meta = (ClampMin = 0.0f, ClampMax = 1.0f, UIMin = 0.0f, UIMax = 1.0f))
	float FallAirControl = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Fall", meta = (ClampMin = 0.0f))
	float FallGravityScale = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Fall", meta = (ClampMin = 0.0f))
	float FallFastGravityScale = 10.0f;
};

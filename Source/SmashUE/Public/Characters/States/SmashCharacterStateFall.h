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
	UPROPERTY(EditDefaultsOnly, Category = "Fall", meta = (ClampMin = 0.0f))
	float FallHorizontalMoveSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Fall", meta = (ClampMin = 0.0f, ClampMax = 1.0f, UIMin = 0.0f, UIMax = 1.0f))
	float FallAirControl = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Fall", meta = (ClampMin = 0.0f))
	float FallGravityScale = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Fall", meta = (ClampMin = 0.0f))
	float FallFastGravityScale = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Fall", DisplayName = "CoyoteTime", meta = (ClampMin = 0.0f, Units = "s"))
	float CoyoteTimeMaxDuration = 0.3f;

private:
	UPROPERTY()
	int MaxJumpCount;

	UPROPERTY()
	int JumpCount;

	UPROPERTY()
	bool bReleasedJumpKey = false;

	UPROPERTY()
	bool bCanDoCoyoteTime = false;

	UPROPERTY()
	float CoyoteTime = 0.0f;

public:
	void SetMaxJumpCount(int Count){MaxJumpCount = Count;}

	void SetJumpCount(int Count) {JumpCount = Count;}
};

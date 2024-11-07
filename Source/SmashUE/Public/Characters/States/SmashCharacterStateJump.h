// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "SmashCharacterStateJump.generated.h"


UCLASS(ClassGroup = (SmashCharacterState), meta = (BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateJump : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual ESmashCharacterStateID GetStateID() override;

	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	virtual void StateExit(ESmashCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Jump", meta = (ClampMin = 0.0f))
	float JumpWalkSpeed = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Jump", meta = (ClampMin = 0.0f))
	float JumpMaxHeight = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Jump")
	bool bUseAnimDuration = true;

	UPROPERTY(EditAnywhere, Category = "Jump", meta = (ClampMin = 0.0f, Units = "s", EditCondition = "!bUseAnimDuration"))
	float JumpDuration = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Jump", meta = (ClampMin = 0.0f, ClampMax = 1.0f, UIMin = 0.0f, UIMax = 1.0f))
	float JumpAirControl = 1.0f;

private:
	float JumpTimer = 0.0f;

	float TargetJumpDuration;
};

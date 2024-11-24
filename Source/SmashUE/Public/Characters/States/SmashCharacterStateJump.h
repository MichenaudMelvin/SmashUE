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
	virtual ESmashCharacterStateID GetStateID() override;

	virtual void StateInit(USmashCharacterStateMachine* InStateMachine) override;

	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	virtual void StateExit(ESmashCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;

	virtual void PlayStateAnimation() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Jump", meta = (ClampMin = 0.0f, ForceUnits = "cm/s"))
	float JumpWalkSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Jump", meta = (ClampMin = 0.0f, ForceUnits = "cm/s"))
	float JumpMaxHeight = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Jump")
	bool bUseAnimDuration = true;

	UPROPERTY(EditDefaultsOnly, Category = "Jump", meta = (ClampMin = 0.0f, Units = "s", EditCondition = "!bUseAnimDuration"))
	float JumpDuration = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Jump", meta = (ClampMin = 0.0f, ClampMax = 1.0f, UIMin = 0.0f, UIMax = 1.0f))
	float JumpAirControl = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Jump", meta = (ClampMin = 1f, ClampMax = 10f, UIMin = 1f, UIMax = 10f))
	int MaxJumpCount = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Animation", meta = (ToolTip = "If NULL, will use StateAnimation", EditCondition = "MaxJumpCount > 1"))
	TObjectPtr<UAnimMontage> OtherJumpsAnimation;

private:
	UPROPERTY()
	float JumpVelocity;

	UPROPERTY()
	float JumpGravity;

	UPROPERTY()
	int JumpCount = 0;

public:
	void SetJumpCount(int Count) {JumpCount = Count;}
};

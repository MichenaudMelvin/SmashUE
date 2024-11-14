// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CameraWorldSubsystem.generated.h"

class UCameraComponent;

UCLASS()
class SMASHUE_API UCameraWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

#pragma region SubsystemOverrides

public:
	virtual void PostInitialize() override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override {return TStatId();}

#pragma endregion

#pragma region MainCamera

protected:
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraMain;

	void TickUpdateCameraPosition(float DeltaTime);

	void TickUpdateCameraZoom(float DeltaTime);

#pragma endregion

#pragma region FollowTargets

protected:
	UPROPERTY()
	TArray<UObject*> FollowTargets;

public:
	void AddFollowTarget(UObject* FollowTarget);

	void RemoveFollowTarget(UObject* FollowTarget);

	float CalculateGreatestDistanceBetweenTargets();

#pragma endregion

#pragma region Bounds

protected:
	UPROPERTY()
	FVector2D ArenaBoundsMin;

	UPROPERTY()
	FVector2D ArenaBoundsMax;

	UPROPERTY()
	float CameraBoundsYProjectionCenter;

	AActor* FindCameraBoundsActor();

	void InitCameraBounds(AActor* CameraBoundsActor);

	void ClampCameraPositionIntoCameraBounds(FVector& Position);

	void GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax);

	FVector CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition);

#pragma endregion

#pragma region Zoom

protected:
	UPROPERTY()
	float CameraZoomYMin = 0.0f;

	UPROPERTY()
	float CameraZoomYMax = 0.0f;

	UPROPERTY()
	float CameraZoomDistanceBetweenTargetsMin = 300.0f;

	UPROPERTY()
	float CameraZoomDistanceBetweenTargetsMax = 1500.0f;

	UFUNCTION()
	void InitCameraZoomParameters();

#pragma endregion

#pragma region Misc

protected:
	UCameraComponent* FindCameraByTag(const FName& Tag) const;

	FVector CalculateAveragePositionBetweenTargets();

#pragma endregion
};

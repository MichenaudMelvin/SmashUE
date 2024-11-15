// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CameraSettings.generated.h"

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Camera Settings"))
class SMASHUE_API UCameraSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Camera")
	FName CameraMainTag = "CameraMain";

	UPROPERTY(Config, EditAnywhere, Category = "Camera")
	FName CameraDistanceMinTag = "CameraDistanceMin";

	UPROPERTY(Config, EditAnywhere, Category = "Camera")
	FName CameraDistanceMaxTag = "CameraDistanceMax";

	UPROPERTY(Config, EditAnywhere, Category = "Camera")
	FName CameraBoundsTag = "CameraBounds";

	UPROPERTY(Config, EditAnywhere, Category = "Camera", meta = (ClampMin = 0.0f))
	float DistanceBetweenTargetsMin = 300.0f;

	UPROPERTY(Config, EditAnywhere, Category = "Camera", meta = (ClampMin = 0.0f))
	float DistanceBetweenTargetsMax = 1500.0f;

	UPROPERTY(Config, EditAnywhere, Category = "Camera", meta = (ClampMin = 0.0f))
	float SizeDampingFactor = 5.0f;

	UPROPERTY(Config, EditAnywhere, Category = "Camera", meta = (ClampMin = 0.0f))
	float PositionDampingFactor = 5.0f;
};

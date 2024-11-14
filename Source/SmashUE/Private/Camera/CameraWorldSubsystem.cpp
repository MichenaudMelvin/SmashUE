// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraFollowTarget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UCameraWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CameraMain = FindCameraByTag(TEXT("CameraMain"));

	AActor* CameraBoundsActor = FindCameraBoundsActor();
	InitCameraZoomParameters();

	if(CameraBoundsActor == nullptr)
	{
		return;
	}

	InitCameraBounds(CameraBoundsActor);
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraZoom(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
}

void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
	if(CameraMain == nullptr)
	{
		return;
	}

	FVector AveragePosition = CalculateAveragePositionBetweenTargets();
	ClampCameraPositionIntoCameraBounds(AveragePosition);

	AveragePosition.Y = CameraMain->GetComponentLocation().Y;

	CameraMain->SetWorldLocation(AveragePosition);
}

void UCameraWorldSubsystem::TickUpdateCameraZoom(float DeltaTime)
{
	if(CameraMain == nullptr)
	{
		return;
	}

	float GreatestDistanceBetweenTargets = CalculateGreatestDistanceBetweenTargets();

	float Percent = UKismetMathLibrary::NormalizeToRange(GreatestDistanceBetweenTargets, CameraZoomDistanceBetweenTargetsMin, CameraZoomDistanceBetweenTargetsMax);
	Percent = FMath::Clamp(Percent, 0.0f, 1.0f);

	FVector CurrentLocation = CameraMain->GetComponentLocation();
	CurrentLocation.Y = FMath::Lerp(CameraZoomYMin, CameraZoomYMax, Percent);

	CameraMain->SetWorldLocation(CurrentLocation);
}

void UCameraWorldSubsystem::AddFollowTarget(UObject* FollowTarget)
{
	if (FollowTarget == nullptr)
	{
		return;
	}

	FollowTargets.Add(FollowTarget);
}

void UCameraWorldSubsystem::RemoveFollowTarget(UObject* FollowTarget)
{
	if (FollowTarget == nullptr)
	{
		return;
	}

	FollowTargets.Remove(FollowTarget);
}

float UCameraWorldSubsystem::CalculateGreatestDistanceBetweenTargets()
{
	float GreatestDistance = 0.0f;

	for (int i = 0; i < FollowTargets.Num(); i++)
	{
		UObject* TargetA = FollowTargets[i];
		if(!TargetA->Implements<UCameraFollowTarget>())
		{
			continue;
		}

		ICameraFollowTarget* FollowTargetInterfaceA = Cast<ICameraFollowTarget>(TargetA);

		if(!FollowTargetInterfaceA->IsFollowable())
		{
			continue;
		}

		for (int j = 0; j < FollowTargets.Num(); j++)
		{
			if(i == j)
			{
				continue;
			}

			UObject* TargetB = FollowTargets[j];
			if(!TargetB->Implements<UCameraFollowTarget>())
			{
				continue;
			}

			ICameraFollowTarget* FollowTargetInterfaceB = Cast<ICameraFollowTarget>(TargetB);

			if(!FollowTargetInterfaceB->IsFollowable())
			{
				continue;
			}

			float Distance = FVector::Dist(FollowTargetInterfaceA->GetFollowPosition(), FollowTargetInterfaceB->GetFollowPosition());

			if(Distance > GreatestDistance)
			{
				GreatestDistance = Distance;
			}
		}
	}

	return GreatestDistance;
}

AActor* UCameraWorldSubsystem::FindCameraBoundsActor()
{
	TArray<AActor*> FoundActors;
	FName Tag = "CameraBounds";
	UGameplayStatics::GetAllActorsWithTag(this, Tag, FoundActors);

	if(FoundActors.Num() == 0)
	{
		return nullptr;
	}

#if WITH_EDITOR
	if(FoundActors.Num() > 1)
	{
		const FString Message = FString::Printf(TEXT("More than one actors have the tag %s, only one is needed."), *Tag.ToString());

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);
		FMessageLog("BlueprintLog").Warning(FText::FromString(Message));
	}
#endif

	return FoundActors[0];
}

void UCameraWorldSubsystem::InitCameraBounds(AActor* CameraBoundsActor)
{
	FVector BoundsCenter;
	FVector BoundsExtents;

	CameraBoundsActor->GetActorBounds(false, BoundsCenter, BoundsExtents);

	CameraBoundsYProjectionCenter = BoundsCenter.Y;
	ArenaBoundsMin.X = BoundsCenter.X - BoundsExtents.X;
	ArenaBoundsMin.Y = BoundsCenter.Z - BoundsExtents.Z;

	ArenaBoundsMax.X = BoundsCenter.X + BoundsExtents.X;
	ArenaBoundsMax.Y = BoundsCenter.Z + BoundsExtents.Z;
}

void UCameraWorldSubsystem::ClampCameraPositionIntoCameraBounds(FVector& Position)
{
	FVector2D ViewportBoundsMin, ViewportBoundsMax;
	GetViewportBounds(ViewportBoundsMin, ViewportBoundsMax);

	FVector FrustumBoundsMin = CalculateWorldPositionFromViewportPosition(ViewportBoundsMin);
	FVector FrustumBoundsMax = CalculateWorldPositionFromViewportPosition(ViewportBoundsMax);

	FVector2D FrustumBoundsSize;
	FrustumBoundsSize.X = (FrustumBoundsMax.X - FrustumBoundsMin.X) / 2.0f;
	FrustumBoundsSize.Y = (FrustumBoundsMin.Z - FrustumBoundsMax.Z) / 2.0f;

	float MinX = ArenaBoundsMin.X + FrustumBoundsSize.X;
	float MaxX = ArenaBoundsMax.X - FrustumBoundsSize.X;

	float MinY = ArenaBoundsMin.Y + FrustumBoundsSize.Y;
	float MaxY = ArenaBoundsMax.Y - FrustumBoundsSize.Y;

	Position.X = FMath::Clamp(Position.X, MinX, MaxX);
	Position.Z = FMath::Clamp(Position.Z, MinY, MaxY);
}

void UCameraWorldSubsystem::GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax)
{
	// Find viewport
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();

	if(ViewportClient == nullptr)
	{
		return;
	}

	FViewport* Viewport = ViewportClient->Viewport;

	if(Viewport == nullptr)
	{
		return;
	}

	// Calculate Viewport Rect according to Camera Aspect Ratio and Viewport ViewRect
	FIntRect ViewRect(
		Viewport->GetInitialPositionXY(),
		Viewport->GetInitialPositionXY() + Viewport->GetSizeXY()
	);
	FIntRect ViewportRect = Viewport->CalculateViewExtents(CameraMain->AspectRatio, ViewRect);

	// Fill Output parameters with ViewportRect
	OutViewportBoundsMin.X = ViewportRect.Min.X;
	OutViewportBoundsMin.Y = ViewportRect.Min.Y;

	OutViewportBoundsMax.X = ViewportRect.Max.X;
	OutViewportBoundsMax.Y = ViewportRect.Max.Y;
}

FVector UCameraWorldSubsystem::CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition)
{
	if(CameraMain == nullptr)
	{
		return FVector::ZeroVector;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if(PlayerController == nullptr)
	{
		return FVector::ZeroVector;
	}

	float YDistanceToCenter = CameraMain->GetOwner()->GetActorLocation().Y - CameraBoundsYProjectionCenter;

	FVector CameraWorldProjectDir;
	FVector WorldPosition;

	UGameplayStatics::DeprojectScreenToWorld(
		PlayerController,
		ViewportPosition,
		WorldPosition,
		CameraWorldProjectDir
	);

	WorldPosition += CameraWorldProjectDir * YDistanceToCenter;

	return WorldPosition;
}

void UCameraWorldSubsystem::InitCameraZoomParameters()
{
	UCameraComponent* CameraDistanceMin = FindCameraByTag("CameraDistanceMin");
	UCameraComponent* CameraDistanceMax = FindCameraByTag("CameraDistanceMax");
	CameraZoomYMin = CameraDistanceMin == nullptr ? 0.0f : CameraDistanceMin->GetComponentLocation().Y;
	CameraZoomYMax = CameraDistanceMax == nullptr ? 0.0f : CameraDistanceMax->GetComponentLocation().Y;
}

UCameraComponent* UCameraWorldSubsystem::FindCameraByTag(const FName& Tag) const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(this, Tag, FoundActors);

	if(FoundActors.Num() == 0)
	{
		return nullptr;
	}

#if WITH_EDITOR
	if(FoundActors.Num() > 1)
	{
		const FString Message = FString::Printf(TEXT("More than one actors have the tag %s, only one is needed."), *Tag.ToString());

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);
		FMessageLog("BlueprintLog").Warning(FText::FromString(Message));
	}
#endif

	UActorComponent* FoundComp = FoundActors[0]->FindComponentByClass(UCameraComponent::StaticClass());

	return Cast<UCameraComponent>(FoundComp);
}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets()
{
	FVector AveragePosition;

	for (UObject* FollowTarget : FollowTargets)
	{
		if(!FollowTarget->Implements<UCameraFollowTarget>())
		{
			continue;
		}

		ICameraFollowTarget* FollowTargetInterface = Cast<ICameraFollowTarget>(FollowTarget);

		if(!FollowTargetInterface->IsFollowable())
		{
			continue;
		}

		AveragePosition += FollowTargetInterface->GetFollowPosition() / FollowTargets.Num();
	}

	return AveragePosition;
}

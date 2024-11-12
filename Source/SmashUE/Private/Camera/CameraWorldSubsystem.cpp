// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"

#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

void UCameraWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CameraMain = FindCameraByTag(TEXT("CameraMain"));
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
}

void UCameraWorldSubsystem::AddFollowTarget(AActor* FollowTarget)
{
	if (FollowTarget == nullptr)
	{
		return;
	}

	FollowTargets.Add(FollowTarget);
}

void UCameraWorldSubsystem::RemoveFollowTarget(AActor* FollowTarget)
{
	if (FollowTarget == nullptr)
	{
		return;
	}

	FollowTargets.Remove(FollowTarget);
}

void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
	if(CameraMain == nullptr)
	{
		return;
	}

	FVector AveragePosition = CalculateAveragePositionBetweenTargets();
	FVector CurrentWorldLocation = CameraMain->GetComponentLocation();
	FVector ForwardVector = CameraMain->GetForwardVector().GetAbs();

	CurrentWorldLocation *= ForwardVector;
	AveragePosition *= (ForwardVector - 1).GetAbs();

	CameraMain->SetWorldLocation(AveragePosition + CurrentWorldLocation);
}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets()
{
	FVector AveragePosition;

	for (const AActor* FollowTarget : FollowTargets)
	{
		AveragePosition += (FollowTarget->GetActorLocation()) * 1/FollowTargets.Num();
	}

	return AveragePosition;
}

UCameraComponent* UCameraWorldSubsystem::FindCameraByTag(const FName& Tag) const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(this, Tag, FoundActors);

	for (const AActor* Actor : FoundActors)
	{
		UActorComponent* FoundComp = Actor->FindComponentByClass(UCameraComponent::StaticClass());

		if(FoundComp != nullptr)
		{
			return Cast<UCameraComponent>(FoundComp);
		}
	}

	return nullptr;
}
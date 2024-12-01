// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Jammus/EnergyBall.h"
#include "Kismet/KismetMathLibrary.h"

AEnergyBall::AEnergyBall()
{
	EnergyBallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnergyBall"));
	SetRootComponent(EnergyBallMesh);

	EnergyBallMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	EnergyBallMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	EnergyBallMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	EnergyBallMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	PrimaryActorTick.bCanEverTick = true;
}

void AEnergyBall::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(EnergyBallState == EEnergyBallState::None || EnergyBallState == EEnergyBallState::Charged)
	{
		return;
	}

	if(EnergyBallState == EEnergyBallState::Charging)
	{
		ChargeEnergy(DeltaSeconds);
	}
	else
	{
		Throw(DeltaSeconds);
	}
}

#if WITH_EDITORONLY_DATA
void AEnergyBall::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName& PropertyName = PropertyChangedEvent.GetMemberPropertyName();

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AEnergyBall, DebugChargingPercent) || PropertyName == GET_MEMBER_NAME_CHECKED(AEnergyBall, MeshScale))
	{
		float ScaleValue = FMath::Lerp(MeshScale.GetLowerBoundValue(), MeshScale.GetUpperBoundValue(), DebugChargingPercent);
		EnergyBallMesh->SetWorldScale3D(FVector(ScaleValue));
	}
}
#endif

void AEnergyBall::InitEnergyBall(const FEnergyBallData& EnergyBallData)
{
	SetEnergyBallState(EEnergyBallState::Charging);
	UpdateEnergyBallData(EnergyBallData);

	FVector TargetMeshScale(MeshScale.GetLowerBoundValue());
	EnergyBallMesh->SetWorldScale3D(TargetMeshScale);

	ChargingDuration = 0.0f;
}

void AEnergyBall::UpdateEnergyBallData(const FEnergyBallData& EnergyBallData)
{
	CurrentData = EnergyBallData;
}

void AEnergyBall::ChargeEnergy(float DeltaTime)
{
	ChargingDuration += DeltaTime;

	float Alpha = UKismetMathLibrary::NormalizeToRange(ChargingDuration, 0.0f, CurrentData.MaxChargingDuration);

	float TargetScale = FMath::Lerp(EnergyBallMesh->GetComponentScale().X, MeshScale.GetUpperBoundValue(), Alpha);
	FVector TargetMeshScale(TargetScale);
	EnergyBallMesh->SetWorldScale3D(TargetMeshScale);

	if(ChargingDuration >= CurrentData.MaxChargingDuration)
	{
		SetEnergyBallState(EEnergyBallState::Charged);
	}
}

void AEnergyBall::CompleteCharge(){}

void AEnergyBall::Throw(float DeltaTime)
{
	FVector Offset = FVector::ForwardVector * CurrentData.ThrowDirection * CurrentData.EnergyBallSpeed;

	AddActorWorldOffset(Offset, true);

	if(!EnergyBallMesh->WasRecentlyRendered())
	{
		Destroy();
	}
}

void AEnergyBall::SetEnergyBallState(const EEnergyBallState& NewState)
{
	EnergyBallState = NewState;

	switch (NewState)
	{
	case EEnergyBallState::None:
		break;
	case EEnergyBallState::Charging:
		EnergyBallMesh->SetVisibility(true, true);
		EnergyBallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EEnergyBallState::Charged:
		EnergyBallMesh->SetVisibility(false, true);
		EnergyBallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CompleteCharge();
		break;
	case EEnergyBallState::Throwing:
		EnergyBallMesh->SetVisibility(true, true);
		EnergyBallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	}
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Jammus/JammusStateNeutralSA.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterStateMachine.h"

void UJammusStateNeutralSA::StateInit(USmashCharacterStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);

	if(EnergyBallClass == nullptr)
	{
		EnergyBallClass = AEnergyBall::StaticClass();
	}

	TargetEnergyBallData.MaxChargingDuration = ChargingDuration;
	TargetEnergyBallData.EnergyBallSpeed = EnergyBallSpeed;

	TArray<UPrimitiveComponent*> Components;
	Character->GetComponents(UPrimitiveComponent::StaticClass(), Components);
	for (UPrimitiveComponent* Component : Components)
	{
		if(Component->DoesSocketExist(SocketName))
		{
			EnergyBallComponent = Component;
			break;
		}
	}

	if(EnergyBallComponent == nullptr)
	{
		EnergyBallComponent = Character->GetMesh();
	}
}

void UJammusStateNeutralSA::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	TargetEnergyBallData.ThrowDirection = Character->GetOrientX();

	if(CurrentEnergyBall != nullptr)
	{
		CurrentEnergyBall->SetEnergyBallState(EEnergyBallState::Throwing);
		CurrentEnergyBall = nullptr;
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	else
	{
		FVector TargetSpawnLocation = EnergyBallComponent->GetSocketLocation(SocketName);
		FRotator TargetRotation = EnergyBallComponent->GetSocketRotation(SocketName);
		FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
		CurrentEnergyBall = GetWorld()->SpawnActor<AEnergyBall>(EnergyBallClass, TargetSpawnLocation, TargetRotation, SpawnParameters);
		CurrentEnergyBall->InitEnergyBall(TargetEnergyBallData);
	}
}

void UJammusStateNeutralSA::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	if(CurrentEnergyBall != nullptr && CurrentEnergyBall->GetEnergyBallState() == EEnergyBallState::Charging)
	{
		CurrentEnergyBall->SetEnergyBallState(EEnergyBallState::Throwing);
		CurrentEnergyBall = nullptr;
	}
}

void UJammusStateNeutralSA::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if(Character->GetInputSpecialAttack())
	{
		if(CurrentEnergyBall->GetEnergyBallState() == EEnergyBallState::Charging)
		{
			FVector TargetLocation = EnergyBallComponent->GetSocketLocation(SocketName);
			CurrentEnergyBall->SetActorLocation(TargetLocation);
		}
	}

	else if(!Character->GetInputSpecialAttack() || CurrentEnergyBall->GetEnergyBallState() == EEnergyBallState::Charged)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
}

#if WITH_EDITORONLY_DATA
void UJammusStateNeutralSA::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName& PropertyName = PropertyChangedEvent.GetMemberPropertyName();

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UJammusStateNeutralSA, DebugChargingPercent) || PropertyName == GET_MEMBER_NAME_CHECKED(UJammusStateNeutralSA, Damages))
	{
		DebugDamagesAtChargingPercent = FMath::Lerp(0.0f, Damages, DebugChargingPercent);
	}
}
#endif
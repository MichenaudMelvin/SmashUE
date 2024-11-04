// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacter.h"


ASmashCharacter::ASmashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASmashCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASmashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASmashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


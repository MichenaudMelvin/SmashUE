// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CharactersSelectionGameMode.generated.h"

UCLASS()
class SMASHUE_API ACharactersSelectionGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ACharactersSelectionGameMode();

protected:
	virtual void BeginPlay() override;

	void CreateAndInitPlayers();

	int GetNumberOfPlayer() const;

	void SpawnPawns(int PawnNumber);

public:
	void LaunchGame();
};

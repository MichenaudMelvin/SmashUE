// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SmashGameInstance.generated.h"

UCLASS()
class SMASHUE_API USmashGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	TArray<FName> PlayersIDs;

public:
	UFUNCTION(BlueprintCallable, Category = "Smash Characters")
	void AddPlayerID(const FName& PlayerID);

	UFUNCTION(BlueprintCallable, Category = "Smash Characters")
	void RemovePlayerID(const FName& PlayerID);

	const TArray<FName>& GetPlayersIDs() const {return PlayersIDs;}
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "SmashGameInstance.h"

void USmashGameInstance::AddPlayerID(const FName& PlayerID)
{
	PlayersIDs.Add(PlayerID);
}

void USmashGameInstance::RemovePlayerID(const FName& PlayerID)
{
	PlayersIDs.Remove(PlayerID);
}

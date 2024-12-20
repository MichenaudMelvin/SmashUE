// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/MatchGameMode.h"
#include "Arena/ArenaPlayerStart.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/SmashCharacterInputData.h"
#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "SmashGameInstance.h"

void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();

	CreateAndInitPlayers();
	TArray<AArenaPlayerStart*> PlayerStartsPoints;
	FindPlayerStartActorsInArena(PlayerStartsPoints);
	SpawnCharacters(PlayerStartsPoints);
}

USmashCharacterInputData* AMatchGameMode::LoadInputDataFromConfig()
{
	const USmashCharacterSettings* CharacterSettings = GetDefault<USmashCharacterSettings>();

	if(CharacterSettings == nullptr)
	{
		return nullptr;
	}

	return CharacterSettings->InputData.LoadSynchronous();
}

void AMatchGameMode::FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*>& ResultActors)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, AArenaPlayerStart::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		AArenaPlayerStart* ArenaPlayerStartActor = Cast<AArenaPlayerStart>(FoundActor);
		if(ArenaPlayerStartActor == nullptr)
		{
			continue;
		}

		ResultActors.Add(ArenaPlayerStartActor);
	}
}

void AMatchGameMode::CreateAndInitPlayers() const
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if(GameInstance == nullptr)
	{
		return;
	}

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if(LocalMultiplayerSubsystem == nullptr)
	{
		return;
	}

	LocalMultiplayerSubsystem->CreateAndInitPlayers(ELocalMultiplayerInputMappingType::InGame);
}

void AMatchGameMode::SpawnCharacters(const TArray<AArenaPlayerStart*>& SpawnPoints)
{
	USmashCharacterInputData* InputData = LoadInputDataFromConfig();
	const USmashCharacterSettings* CharacterSettings = GetDefault<USmashCharacterSettings>();
	const TArray<FName>& PlayersIDs = Cast<USmashGameInstance>(GetGameInstance())->GetPlayersIDs();

	for (int i = 0; i < PlayersIDs.Num(); ++i)
	{
		const FName& PlayerID = PlayersIDs[i];
		AArenaPlayerStart* SpawnPoint = SpawnPoints[i];

		TSubclassOf<ASmashCharacter> SmashCharacterClass = CharacterSettings->GetCharacterClassFromCharacterID(PlayerID);

		if(SmashCharacterClass == nullptr)
		{
			continue;
		}

		ASmashCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<ASmashCharacter>(
			SmashCharacterClass,
			SpawnPoint->GetTransform()
		);

		if(NewCharacter == nullptr)
		{
			continue;
		}

		NewCharacter->InputData = InputData;
		NewCharacter->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;

		NewCharacter->SetOrientX(SpawnPoint->GetStartOrientX());
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());

		CharactersInsideArena.Add(NewCharacter);
	}
}

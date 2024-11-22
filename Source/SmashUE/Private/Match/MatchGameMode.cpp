// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/MatchGameMode.h"
#include "Arena/ArenaPlayerStart.h"
#include "Arena/ArenaSettings.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/SmashCharacterInputData.h"
#include "InputMappingContext.h"
#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"

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

	//GetDefault<UArenaSettings>()->NumbersOfPlayers is temp
	for(int i = 0; i < GetDefault<UArenaSettings>()->NumbersOfPlayers; i++)
	// for (AArenaPlayerStart* SpawnPoint : SpawnPoints)
	{
		AArenaPlayerStart* SpawnPoint = SpawnPoints[i];

		EAutoReceiveInput::Type InputType = SpawnPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> SmashCharacterClass = GetSmashCharacterClassFromInputType(InputType);

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

TSubclassOf<ASmashCharacter> AMatchGameMode::GetSmashCharacterClassFromInputType(EAutoReceiveInput::Type InputType) const
{
	const UArenaSettings* ArenaSettings = GetDefault<UArenaSettings>();

	switch (InputType)
	{
	case EAutoReceiveInput::Player0:
		return ArenaSettings->SmashCharacterClassP0;

	case EAutoReceiveInput::Player1:
		return ArenaSettings->SmashCharacterClassP1;

	case EAutoReceiveInput::Player2:
		return ArenaSettings->SmashCharacterClassP2;

	case EAutoReceiveInput::Player3:
		return ArenaSettings->SmashCharacterClassP3;

	default:
		return nullptr;
	}
}

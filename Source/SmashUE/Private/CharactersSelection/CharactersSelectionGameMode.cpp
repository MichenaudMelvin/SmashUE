// Fill out your copyright notice in the Description page of Project Settings.


#include "CharactersSelection/CharactersSelectionGameMode.h"
#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "Animation/SkeletalMeshActor.h"
#include "CharactersSelection/CharacterSelectionPawn.h"
#include "CharactersSelection/CharacterSelectionSettings.h"
#include "CharactersSelection/CharacterSelectionInputData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ACharactersSelectionGameMode::ACharactersSelectionGameMode()
{
	DefaultPawnClass = nullptr;
}

void ACharactersSelectionGameMode::BeginPlay()
{
	Super::BeginPlay();

	CreateAndInitPlayers();
	SpawnPawns(GetNumberOfPlayer());
}

void ACharactersSelectionGameMode::CreateAndInitPlayers()
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

	LocalMultiplayerSubsystem->CreateAndInitPlayers(ELocalMultiplayerInputMappingType::Menu);
}

int ACharactersSelectionGameMode::GetNumberOfPlayer() const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, ASkeletalMeshActor::StaticClass(), FoundActors);
	return FoundActors.Num();
}

void ACharactersSelectionGameMode::SpawnPawns(int PawnNumber)
{
	const UCharacterSelectionSettings* Settings = GetDefault<UCharacterSelectionSettings>();
	if(Settings == nullptr)
	{
		return;
	}

	TSubclassOf<ACharacterSelectionPawn> PawnClass = Settings->CharacterSelectionClass;
	if(PawnClass == nullptr)
	{
		PawnClass = ACharacterSelectionPawn::StaticClass();
	}

	UCharacterSelectionInputData* InputData = Settings->InputData.LoadSynchronous();
	if(InputData == nullptr)
	{
		return;
	}

	for(int i = 0; i < PawnNumber; i++)
	{
		ACharacterSelectionPawn* NewPawn = GetWorld()->SpawnActorDeferred<ACharacterSelectionPawn>(PawnClass, FTransform::Identity);
		if(NewPawn == nullptr)
		{
			continue;
		}

		NewPawn->SetInputData(InputData);
		NewPawn->AutoPossessPlayer = static_cast<EAutoReceiveInput::Type>(i + 1); // 0 is disabled
		NewPawn->FinishSpawning(FTransform::Identity);
	}
}

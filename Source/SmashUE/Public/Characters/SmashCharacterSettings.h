// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmashCharacter.h"
#include "SmashCharacterStateID.h"
#include "Engine/DeveloperSettings.h"
#include "SmashCharacterSettings.generated.h"

class USmashCharacterState;
class UInputMappingContext;
class USmashCharacterInputData;

USTRUCT(BlueprintType, Category = "Smash Character")
struct FSmashCharacterData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Data")
	FName ID = "";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Data")
	TSubclassOf<ASmashCharacter> Class = ASmashCharacter::StaticClass();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Data")
	TObjectPtr<USkeletalMesh> Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Data")
	TObjectPtr<UMaterial> Material = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Data")
	TObjectPtr<UTexture2D> SmallAvatar = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Data")
	TObjectPtr<UTexture2D> MediumAvatar = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Data")
	TObjectPtr<UTexture2D> Badge = nullptr;
};

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Smash Character Settings"))
class SMASHUE_API USmashCharacterSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Inputs")
	TSoftObjectPtr<USmashCharacterInputData> InputData;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Inputs", meta = (ClampMin = 0.0f, ClampMax = 1.0f, UIMin = 0.0f, UIMax = 1.0f))
	float InputMoveXThreshold = 0.1f;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Inputs", meta = (ClampMin = 0.0f, ClampMax = 1.0f, UIMin = 0.0f, UIMax = 1.0f))
	float InputMoveYThreshold = 0.1f;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Orient", DisplayName = "2D Orient")
	bool b2DOrient = true;

	UPROPERTY(Config, EditDefaultsOnly, Category = "States", DisplayName = "Default Character States")
	TMap<ESmashCharacterStateID, TSubclassOf<USmashCharacterState>> CharacterStates;

protected:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Character Data")
	TArray<FSmashCharacterData> CharactersData;

public:
	const FSmashCharacterData GetCharacterDataFromCharacterID(const FName& CharacterID) const;

	TSubclassOf<ASmashCharacter> GetCharacterClassFromCharacterID(const FName& CharacterID) const;

	USkeletalMesh* GetSkeletalMeshFromCharacterID(const FName& CharacterID) const;

	UMaterial* GetMaterialFromCharacterID(const FName& CharacterID) const;

	UTexture2D* GetSmallAvatarFromCharacterID(const FName& CharacterID) const;

	UTexture2D* GetMediumAvatarFromCharacterID(const FName& CharacterID) const;

	UTexture2D* GetBadgeFromCharacterID(const FName& CharacterID) const;
};

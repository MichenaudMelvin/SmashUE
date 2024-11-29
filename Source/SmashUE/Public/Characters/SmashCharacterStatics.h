// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SmashCharacterStatics.generated.h"

struct FSmashCharacterData;

UCLASS()
class SMASHUE_API USmashCharacterStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Smash Character Data")
	static FSmashCharacterData GetCharacterDataFromCharacterID(UPARAM(ref) const FName& CharacterID);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Smash Character Data")
	static TSubclassOf<ASmashCharacter> GetCharacterClassFromCharacterID(const FName& CharacterID);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Smash Character Data")
	static USkeletalMesh* GetSkeletalMeshFromCharacterID(UPARAM(ref) const FName& CharacterID);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Smash Character Data")
	static UMaterial* GetMaterialFromCharacterID(UPARAM(ref) const FName& CharacterID);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Smash Character Data")
	static UTexture2D* GetSmallAvatarFromCharacterID(UPARAM(ref) const FName& CharacterID);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Smash Character Data")
	static UTexture2D* GetMediumAvatarFromCharacterID(UPARAM(ref) const FName& CharacterID);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Smash Character Data")
	static UTexture2D* GetBadgeFromCharacterID(UPARAM(ref) const FName& CharacterID);
};

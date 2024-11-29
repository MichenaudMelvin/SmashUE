// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterStatics.h"
#include "Characters/SmashCharacterSettings.h"

FSmashCharacterData USmashCharacterStatics::GetCharacterDataFromCharacterID(const FName& CharacterID)
{
	const USmashCharacterSettings* Settings = GetDefault<USmashCharacterSettings>();
	if(Settings == nullptr)
	{
		return FSmashCharacterData();
	}

	return Settings->GetCharacterDataFromCharacterID(CharacterID);
}

TSubclassOf<ASmashCharacter> USmashCharacterStatics::GetCharacterClassFromCharacterID(const FName& CharacterID)
{
	const USmashCharacterSettings* Settings = GetDefault<USmashCharacterSettings>();
	if(Settings == nullptr)
	{
		return nullptr;
	}

	return Settings->GetCharacterClassFromCharacterID(CharacterID);
}

USkeletalMesh* USmashCharacterStatics::GetSkeletalMeshFromCharacterID(const FName& CharacterID)
{
	const USmashCharacterSettings* Settings = GetDefault<USmashCharacterSettings>();
	if(Settings == nullptr)
	{
		return nullptr;
	}

	return Settings->GetSkeletalMeshFromCharacterID(CharacterID);
}

UMaterial* USmashCharacterStatics::GetMaterialFromCharacterID(const FName& CharacterID)
{
	const USmashCharacterSettings* Settings = GetDefault<USmashCharacterSettings>();
	if(Settings == nullptr)
	{
		return nullptr;
	}

	return Settings->GetMaterialFromCharacterID(CharacterID);
}

UTexture2D* USmashCharacterStatics::GetSmallAvatarFromCharacterID(const FName& CharacterID)
{
	const USmashCharacterSettings* Settings = GetDefault<USmashCharacterSettings>();
	if(Settings == nullptr)
	{
		return nullptr;
	}

	return Settings->GetSmallAvatarFromCharacterID(CharacterID);
}

UTexture2D* USmashCharacterStatics::GetMediumAvatarFromCharacterID(const FName& CharacterID)
{
	const USmashCharacterSettings* Settings = GetDefault<USmashCharacterSettings>();
	if(Settings == nullptr)
	{
		return nullptr;
	}

	return Settings->GetMediumAvatarFromCharacterID(CharacterID);
}

UTexture2D* USmashCharacterStatics::GetBadgeFromCharacterID(const FName& CharacterID)
{
	const USmashCharacterSettings* Settings = GetDefault<USmashCharacterSettings>();
	if(Settings == nullptr)
	{
		return nullptr;
	}

	return Settings->GetBadgeFromCharacterID(CharacterID);
}

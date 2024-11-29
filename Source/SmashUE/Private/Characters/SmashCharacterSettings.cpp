// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterSettings.h"

const FSmashCharacterData USmashCharacterSettings::GetCharacterDataFromCharacterID(const FName& CharacterID) const
{
	for (const FSmashCharacterData& CharacterData : CharactersData)
	{
		if(CharacterData.ID == CharacterID)
		{
			return CharacterData;
		}
	}

	return FSmashCharacterData();
}

TSubclassOf<ASmashCharacter> USmashCharacterSettings::GetCharacterClassFromCharacterID(const FName& CharacterID) const
{
	return GetCharacterDataFromCharacterID(CharacterID).Class;
}

USkeletalMesh* USmashCharacterSettings::GetSkeletalMeshFromCharacterID(const FName& CharacterID) const
{
	return GetCharacterDataFromCharacterID(CharacterID).Mesh;
}

UMaterial* USmashCharacterSettings::GetMaterialFromCharacterID(const FName& CharacterID) const
{
	return GetCharacterDataFromCharacterID(CharacterID).Material;
}

UTexture2D* USmashCharacterSettings::GetSmallAvatarFromCharacterID(const FName& CharacterID) const
{
	return GetCharacterDataFromCharacterID(CharacterID).SmallAvatar;
}

UTexture2D* USmashCharacterSettings::GetMediumAvatarFromCharacterID(const FName& CharacterID) const
{
	return GetCharacterDataFromCharacterID(CharacterID).MediumAvatar;

}

UTexture2D* USmashCharacterSettings::GetBadgeFromCharacterID(const FName& CharacterID) const
{
	return GetCharacterDataFromCharacterID(CharacterID).Badge;
}

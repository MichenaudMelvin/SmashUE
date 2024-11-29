// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UICharacterSelection.h"

#include "UsefulFunctions.h"
#include "CharactersSelection/CharacterSelectionPawn.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UIPlayerCursor.h"
#include "UI/UIPlayerToken.h"

void UUICharacterSelection::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Cursors.Add(CursorP1);
	Cursors.Add(CursorP2);
	Cursors.Add(CursorP3);
	Cursors.Add(CursorP4);

	Tokens.Add(TokenP1);
	Tokens.Add(TokenP2);
	Tokens.Add(TokenP3);
	Tokens.Add(TokenP4);

	TArray<AActor*> OutPawns;
	UGameplayStatics::GetAllActorsOfClass(this, ACharacterSelectionPawn::StaticClass(), OutPawns);
	for (int i = 0; i < OutPawns.Num(); ++i)
	{
		ACharacterSelectionPawn* CurrentPawn = Cast<ACharacterSelectionPawn>(OutPawns[i]);
		Pawns.Add(CurrentPawn);

		Cursors[i]->InitCursor(CurrentPawn, Tokens[i], i);
		Tokens[i]->InitToken(i, this);
	}

	if (CharactersIconPanel == nullptr)
	{
		return;
	}

	CharactersIcons = CharactersIconPanel->GetAllChildren();
}

void UUICharacterSelection::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	for (UUIPlayerCursor* CurrentCursor : Cursors)
	{
		CurrentCursor->TickCursor(MyGeometry, InDeltaTime);
	}
}

bool UUICharacterSelection::DoesTokenOverlapACharacterIcon(const UUIPlayerToken* Token) const
{
	for (const UWidget* CharacterIcon : CharactersIcons)
	{
		if (UUsefulFunctions::DoesWidgetsOverlaps(Token, CharacterIcon))
		{
			return true;
		}
	}

	return false;
}


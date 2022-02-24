// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelect.h"

// Sets default values for this component's properties
UCharacterSelect::UCharacterSelect()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	CharacterSelectData = {};
}

void UCharacterSelect::TrackPlayer(APlayerController * Player, UPARAM(ref) const TArray<TSubclassOf<APawn>>& PlayerAllowedCharacters)
{
	if (GetOwner()->GetWorld()->GetAuthGameMode()) {
		auto ExistingItem = CharacterSelectData.FindByPredicate([&](const FCharacterSelectContainer& i) { return i.Player == Player; });
		if (!ExistingItem) {
			CharacterSelectData.Add(FCharacterSelectContainer(Player, PlayerAllowedCharacters));
		}
	}
}

void UCharacterSelect::UntrackPlayer(APlayerController * Player)
{
	if (GetOwner()->GetWorld()->GetAuthGameMode()) {
		int32 IndexOf = CharacterSelectData.IndexOfByPredicate([&](const FCharacterSelectContainer& i) { return i.Player == Player; });
		if (IndexOf != INDEX_NONE) {
			CharacterSelectData.RemoveAt(IndexOf);
		}
	}
}

void UCharacterSelect::ChooseCharacter_Implementation(APlayerController * Player, TSubclassOf<APawn> Character)
{
	auto ExistingItem = CharacterSelectData.FindByPredicate([&](const FCharacterSelectContainer& i) { return i.Player == Player; });
	if (ExistingItem && Character) {
		if (ExistingItem->AllowedCharacters.FindByPredicate([&](TSubclassOf<APawn> i) { return Character->GetName() == i->GetName();  })) {
			ExistingItem->ChosenCharacter = Character;
		} else {
			OnIllegalCharacter.Broadcast(Player, Character);
		}
	}
}

void UCharacterSelect::ApplyCharacters()
{
	if (GetOwner()->GetWorld()->GetAuthGameMode()) {
		for (const auto& Data : CharacterSelectData) {
			OnCharacterChosen.Broadcast(Data.Player, Data.ChosenCharacter);
		}
	}
}


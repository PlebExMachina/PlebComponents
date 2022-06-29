// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectCompanion.h"
#include "GameFramework/GameStateBase.h"
#include "CharacterSelect.h"
#include "PlebComponentsAPI.h"

// Sets default values for this component's properties
UCharacterSelectCompanion::UCharacterSelectCompanion()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterSelectCompanion::ChooseCharacter_Implementation(TSubclassOf<APawn> Character) {
	// Control transferred to server. Owning connection made. Request character.
	auto PlayerController = Cast<APlayerController>(GetOwner());
	if (PlayerController) {
		auto CharacterSelect = PlebComponentsAPI::GetComponent<UCharacterSelect>(GetWorld()->GetGameState());
		if (CharacterSelect) {
			CharacterSelect->ChooseCharacter(PlayerController,Character);
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectCompanion.h"
#include "GameFramework/GameStateBase.h"
#include "CharacterSelect.h"

// Sets default values for this component's properties
UCharacterSelectCompanion::UCharacterSelectCompanion()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UCharacterSelectCompanion::ChooseCharacter_Implementation(TSubclassOf<APawn> Character) {
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	if (PlayerController) {
		UCharacterSelect* CharacterSelect = Cast<UCharacterSelect>(GetWorld()->GetGameState()->GetComponentByClass(UCharacterSelect::StaticClass()));
		if (CharacterSelect) {
			CharacterSelect->ChooseCharacter(PlayerController,Character);
		}
	}
}
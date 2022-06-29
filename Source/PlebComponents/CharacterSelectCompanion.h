// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterSelectCompanion.generated.h"

/*
* Server RPCs cannot actually be called on the Client if they are on the GameMode or GameState due to a lack
* of owning connections. For this purpose "companion components" will need to exist for the PlayerController to
* contain the API to call these routines.
*
* Companion Components will only function properly if the recommended component attachments are utilized.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLEBCOMPONENTS_API UCharacterSelectCompanion : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterSelectCompanion();

	// Transfer control to server to pick a character.
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ChooseCharacter(TSubclassOf<APawn> Character);
};

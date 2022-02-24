// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterSelect.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCharacterChosenDelegate, APlayerController*, Player, TSubclassOf<APawn>, Character);

USTRUCT()
struct PLEBCOMPONENTS_API FCharacterSelectContainer {
	GENERATED_BODY()

	UPROPERTY()
	APlayerController* Player;

	UPROPERTY()
	TArray<TSubclassOf<APawn> > AllowedCharacters;

	UPROPERTY()
	TSubclassOf<APawn> ChosenCharacter;

	FCharacterSelectContainer() {};
	FCharacterSelectContainer(APlayerController* iPlayer, const TArray<TSubclassOf<APawn>>& iAllowedCharacters) : Player(iPlayer), AllowedCharacters(iAllowedCharacters), ChosenCharacter(nullptr) {};
};

/*
* Handles the tracking of chosen characters for a 'character slect' and also outputs them for processing.
* This should be attatched to AGameState.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLEBCOMPONENTS_API UCharacterSelect : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Player Character Slect Data
	UPROPERTY()
	TArray<FCharacterSelectContainer> CharacterSelectData;

public:	
	// Outputs the relevant controllers / characters for processing.
	UPROPERTY(BlueprintAssignable)
	FCharacterChosenDelegate OnCharacterChosen;

	// Outputs if a player has somehow requested an illegal character. (This is so Validation can be implementation defined.)
	UPROPERTY(BlueprintAssignable)
	FCharacterChosenDelegate OnIllegalCharacter;

	// Sets default values for this component's properties
	UCharacterSelect();

	/*
	* Tracks the relevant player controller (typically used with AGameMode::Login.)
	*
	* Only has an effect if called on server.
	*/
	UFUNCTION(BlueprintCallable)
	void TrackPlayer(APlayerController* Player, UPARAM(ref) const TArray<TSubclassOf<APawn>>& PlayerAllowedCharacters);

	/*
	* Untracks the relevant player controller (typically used with AGameMode::Logout.)
	*
	* Only has an effect if called on server.
	*/
	UFUNCTION(BlueprintCallable)
	void UntrackPlayer(APlayerController* Player);

	/*
	* Called from client to ask the server to update the character list.
	* Only relevant for tracked players.
	*/
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ChooseCharacter(APlayerController* Player, TSubclassOf<APawn> Character);

	// Broadcasts the locked in characters for processing.
	UFUNCTION(BlueprintCallable)
	void ApplyCharacters();
};

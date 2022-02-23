// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorLoader.generated.h"

/*
* This component it to automate the process of spawning in a specific pawn type for the controller.
* This should be called after the player has been properly loaded in.
*
* Since this component will likely be used with Character Selects and the like the timing issues shouldn't be prevalent.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLEBCOMPONENTS_API UActorLoader : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorLoader();

	// Loads the Actor
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void LoadActor(TSubclassOf<APawn> Class, const FTransform& SpawnTransform);
};

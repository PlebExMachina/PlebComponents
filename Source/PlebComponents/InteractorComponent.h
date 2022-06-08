// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractorComponent.generated.h"

/*
	Component to request generic interactions to objects in the world.
	This is intended to be on Player Actors but it should have a use case for AI Actors too.

	The interaction is defined to be as generic as possible but it doesn't take into account additional inputs.
	"Inputs" can be inferred to be a component attached to either the caller's Controller or controlled Actor.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLEBCOMPONENTS_API UInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractorComponent();

protected:
	// Changes to server context to call interface on target. 
	UFUNCTION(Server,Reliable)
	void _ServerInteract(AController* Caller, UObject* Target);

public:	
	// If actor has an owning connection then it calls IInterfaceInteractable::Interact on the target automatically providing context.
	UFUNCTION(BlueprintCallable)
	void TryInteract(UObject* Target);
};

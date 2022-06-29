// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComponentLoader.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnComponentLoadedDelegate, UActorComponent*, Comp, const FName&, Context);

/*
* Handles the Netcode for loading new components onto the owner. 
* !! Inherently unsafe - OnComponentLoaded will need to be used on the server side to make sure nothing malicious is going on.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLEBCOMPONENTS_API UComponentLoader : public UActorComponent
{
	GENERATED_BODY()

protected:
	UFUNCTION(NetMulticast, Reliable)
	void ResolveLoadComponent(TSubclassOf<UActorComponent> Class, const FName& Context);

public:	
	// Sets default values for this component's properties
	UComponentLoader();

	// Loads component onto the actor.
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void LoadComponent(TSubclassOf<UActorComponent> Class, const FName& Context);
		
	// Called after the component properly loads to allow for initialization.
	UPROPERTY(BlueprintAssignable)
	FOnComponentLoadedDelegate OnComponentLoaded;
};

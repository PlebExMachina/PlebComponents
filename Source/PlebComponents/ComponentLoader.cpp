// Fill out your copyright notice in the Description page of Project Settings.


#include "ComponentLoader.h"

// Sets default values for this component's properties
UComponentLoader::UComponentLoader()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UComponentLoader::LoadComponent_Implementation(TSubclassOf<UActorComponent> Class, const FName& Context) {
	ResolveLoadComponent(Class, Context);
}

void UComponentLoader::ResolveLoadComponent_Implementation(TSubclassOf<UActorComponent> Class, const FName& Context) {
	if (GetOwner()) {
		OnComponentLoaded.Broadcast(
			GetOwner()->AddComponentByClass(Class, false, FTransform(), false),
			Context
		);
	}
}
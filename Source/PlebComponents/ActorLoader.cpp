// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorLoader.h"
#include "PlebComponentsAPI.h"

// Sets default values for this component's properties
UActorLoader::UActorLoader()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UActorLoader::LoadActor_Implementation(TSubclassOf<APawn> Class, const FTransform& SpawnTransform) {
	auto Owner = Cast<APlayerController>(GetOwner());
	if (Owner && Class) {
		if (Owner->GetPawn()) {
			Owner->GetPawn()->Destroy();
		}
		Owner->Possess(PlebComponentsAPI::SpawnActor(this, Class, SpawnTransform));
	}
}

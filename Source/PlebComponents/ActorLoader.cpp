// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorLoader.h"

// Sets default values for this component's properties
UActorLoader::UActorLoader()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UActorLoader::LoadActor_Implementation(TSubclassOf<APawn> Class, const FTransform& SpawnTransform) {
	APlayerController* Owner = Cast<APlayerController>(GetOwner());
	if (Owner) {
		if (Owner->GetPawn()) {
			Owner->GetPawn()->Destroy();
		}
		Owner->Possess(Owner->GetWorld()->SpawnActor<APawn>(Class, SpawnTransform.GetLocation(), SpawnTransform.GetRotation().Rotator()));
	}
}

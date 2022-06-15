// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPooler.h"
#include "Pool.h"

namespace {
	auto IsServer = [](UObject* o) -> bool { return o->GetWorld()->GetAuthGameMode() != nullptr; };
};

// Sets default values for this component's properties
UObjectPooler::UObjectPooler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bDynamicPool = false;
	PoolCount = 10;
	//PoolClass = AActor::StaticClass();	
	_Pool = {};
	// ...
}

void UObjectPooler::BeginPlay()
{
	if (IsServer(this)) {
		for (int i = 0; i < PoolCount; ++i) {
			//PoolActor();
		}
	}
}

/*bool UObjectPooler::TrySpawnActor(const FTransform& SpawnTransform)
{
	if (IsServer(this)) {
	
	}
	return false;
}

bool UObjectPooler::TryReleaseActor(AActor* Actor)
{
	if (Actor->GetClass()->ImplementsInterface(UPool::StaticClass())) {
		if (IPool::Execute_GetPoolOwner(Actor) == this) {
			// Handle any needed cleanup.
			IPool::Execute_Release(Actor);
			Actor->SetActorHiddenInGame(true);
			Actor->SetActorEnableCollision(false);
			Actor->SetActorTickEnabled(false);

			// Add to pool.
			if (IsServer(this)) {
				_Pool.Push(Actor);
			}
		}
	}
}*/

void UObjectPooler::PoolActor()
{
	if (IsServer(this)) {

	}
}


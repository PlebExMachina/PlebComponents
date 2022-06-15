// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPooler.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLEBCOMPONENTS_API UObjectPooler : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectPooler();

protected:
	// Adds an actor to the pool. Calls IPool::Initialize on the created actor.
	UFUNCTION()
	void PoolActor();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Tracks which actors are available on server.
	UPROPERTY()
	TArray<AActor*> _Pool;

	// The actor class to pool.
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PoolClass;

	// The initial size of the Object Pool.
	UPROPERTY(EditAnywhere)
	int32 PoolCount;

	// Whether or not the Object Pool should grow on an "On Need" basis.
	UPROPERTY(EditAnywhere)
	bool bDynamicPool;
		
public:
	/*// Attempts to spawn an actor from the pool. Expands pool if needed.
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void TrySpawnActor(const FTransform& SpawnTransform);

	// Attempts to release an actor to the pool. Target needs to implement IPool::GetPoolOwner and PoolOwner needs to be this instance.
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void TryReleaseActor(AActor* Actor);*/
};

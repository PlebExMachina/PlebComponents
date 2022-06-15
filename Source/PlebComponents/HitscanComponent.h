// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitscanComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHitscanEvent, class UHitscanComponent*, Comp, const TArray<FHitResult>&, Hits);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLEBCOMPONENTS_API UHitscanComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastHitResults(const TArray<FHitResult>& HitResults);

public:	
	// Sets default values for this component's properties
	UHitscanComponent();

	// How far away to place the firing lens. The greater the distance the more accurate the firing will be.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float LensDistance;

	// How large the firing lens is. The greater the radius the less accurate the firing will be.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float LensRadius;

	// How far away a shot should be attempted.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float FiringDistance;

	// How many shots to attempt simultaniously.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int FireCount;

	// If set then debug lines will be drawn. (May only work in single player)
	UPROPERTY(EditAnywhere)
	bool bDebug;

	UPROPERTY(BlueprintAssignable)
	FHitscanEvent OnHitscanHit;

	// Server Only, Fires a raycast utilizing the configuration. The raycast result is then multicast and passed 
	UFUNCTION(BlueprintCallable)
	void Fire(const FVector& Origin, const FVector& Target, const TArray<AActor*>& IgnoredActors);
};

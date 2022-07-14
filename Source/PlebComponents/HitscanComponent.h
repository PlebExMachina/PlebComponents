// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitscanComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHitscanEvent, UHitscanComponent*, Comp, const TArray<FHitResult>&, Hits);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLEBCOMPONENTS_API UHitscanComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastHitResults(const TArray<FHitResult>& HitResults);

	// Whether or not the gun is being aimed.
	UPROPERTY(Replicated)
	bool bIsAiming;

	// How much further to place the aiming lens while aiming down sights. 
	UPROPERTY(Replicated, EditAnywhere)
	float AimBonus;

public:	
	// Sets default values for this component's properties
	UHitscanComponent();

	// How far away to place the firing lens. The greater the distance the more accurate the firing will be. Affects the "Minimum Bloom" of the shot.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float LensDistance;

	// How large the firing lens is. The greater the radius the less accurate the firing will be. Affects the "Maximum Bloom" of the shot.
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

	// Executed after hitscan projectiles have hit their mark.
	UPROPERTY(BlueprintAssignable)
	FHitscanEvent OnHitscanHit;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> FiringChannel;

	// Server Only, Fires a raycast utilizing the configuration. The raycast result is then multicast and passed 
	UFUNCTION(BlueprintCallable)
	void Fire(const FVector& Origin, const FVector& Target, const TArray<AActor*>& IgnoredActors);

	// Server Only, Begins Aiming Mode
	UFUNCTION(BlueprintCallable)
	void BeginAim();

	// Server Only, Ends Aiming Mode
	UFUNCTION(BlueprintCallable)
	void EndAim();

	// Sets how much further to place the aiming lens when aiming down sights.
	UFUNCTION(BlueprintCallable)
	void SetAimBonus(float Bonus);

	// Returns whether or not the gun is being aimed down sights.
	UFUNCTION(BlueprintPure)
	bool IsAiming();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitscanComponent.h"
#include "GunComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PLEBCOMPONENTS_API UGunComponent : public UHitscanComponent
{
	GENERATED_BODY()
	
protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* TickFunction) override;

	// The minimum distance the lens can be.
	UPROPERTY(EditAnywhere, Replicated)
	float MinimumLens;

	// The maximum distance the lens can be.
	UPROPERTY(EditAnywhere, Replicated)
	float MaximumLens;

	// How quickly the lens "retracts" which drives minimum bloom towards maximum. (Per Shot)
	UPROPERTY(EditAnywhere, Replicated)
	float Recoil;

	// How quickly the lens "extends" over time restoring accuracy. (Per Second)
	UPROPERTY(EditAnywhere, Replicated)
	float Recovery;

	UPROPERTY()
	USceneComponent* DebugOrigin;

	UPROPERTY()
	FVector DebugForwardVector;


public:
	UGunComponent();

	// The minimum distance the lens can be.
	UFUNCTION(BlueprintCallable)
	void SetMinimumLens(float NewLensSize);

	// The minimum distance the lens can be.
	UFUNCTION(BlueprintPure)
	float GetMinimumLens();

	// The maximum distance the lens can be.
	UFUNCTION(BlueprintCallable)
	void SetMaximumLens(float NewLensSize);

	// The maximum distance the lens can be.
	UFUNCTION(BlueprintPure)
	float GetMaximumLens();

	// How quickly the lens "retracts" which drives minimum bloom towards maximum. (Per Shot)
	UFUNCTION(BlueprintCallable)
	void SetRecoil(float NewRecoil);

	// How quickly the lens "retracts" which drives minimum bloom towards maximum. (Per Shot)
	UFUNCTION(BlueprintPure)
	float GetRecoil();

	/// How quickly the lens "extends" over time restoring accuracy. (Per Second)
	UFUNCTION(BlueprintCallable)
	void SetRecovery(float NewRecovery);

	// How quickly the lens "extends" over time restoring accuracy. (Per Second)
	UFUNCTION(BlueprintPure)
	float GetRecovery();

	// Server Only, Fires a raycast utilizing the configuration. The raycast result is then multicast and passed 
	UFUNCTION(BlueprintCallable)
	void GunFire(USceneComponent* Origin, const FVector& ForwardVector, const TArray<AActor*>& IgnoredActors);
};

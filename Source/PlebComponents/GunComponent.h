// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitscanComponent.h"
#include "GunComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGunComponentEvent, UGunComponent*, Comp);

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PLEBCOMPONENTS_API UGunComponent : public UHitscanComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	USceneComponent* DebugOrigin;

	UPROPERTY()
	FVector DebugForwardVector;

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* TickFunction) override;

	virtual void BeginPlay() override;

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

	// How much ammunition the weapon can have. < 0 for infinite.
	UPROPERTY(EditAnywhere, Replicated)
	int32 MaximumAmmunition;

	// How much ammunition is currently in the weapon.
	UPROPERTY(Replicated)
	int32 CurrentAmmunition;
	
	// How much ammunition the weapon consumes per shot.
	UPROPERTY(Replicated)
	int32 AmmoConsumption;


public:
	UGunComponent();

	// Called whenever a gun is attempted to be fired when out of ammo.
	UPROPERTY(BlueprintAssignable)
	FGunComponentEvent OnOutOfAmmo;

	// Called whenever a reload is attempted on a gun with already full ammo.
	UPROPERTY(BlueprintAssignable)
	FGunComponentEvent OnAlreadyMaxAmmo;

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

	// Server Only, Reloads ammunition. Returns how many bullets were added.
	UFUNCTION(BlueprintCallable)
	int32 Reload();

	// Returns whether or not a reload is possible.
	UFUNCTION(BlueprintPure)
	bool CanReload();
};

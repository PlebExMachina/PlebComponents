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

	// Applies damage to the target actor on Server.
	UFUNCTION()
	void ApplyDamage(UHitscanComponent* Comp, const TArray<FHitResult>& Hits);

	// Plays Muzzle Sound Effect / Muzzle Particle Effect
	UFUNCTION()
	void PlayMuzzleFX(UHitscanComponent* Comp, const TArray<FHitResult>& Hits);

	// Sets it so that the gun can be fired again.
	UFUNCTION()
	void RestoreFire();

	UPROPERTY()
	FTimerHandle RestoreFireHandle;

	// Automatically stops automatic mode when OutOfAmmo triggers.
	UFUNCTION()
	void ForceStopAuto(UGunComponent* Comp);
	
	// The amount of damage a bullet inflicts.
	UPROPERTY(EditAnywhere, Replicated)
	float Damage;

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

	// How often the gun can be fired per second.
	UPROPERTY(Replicated, EditAnywhere)
	float FireRate;

	// Whether or not the gun can currently fire.
	UPROPERTY()
	bool bCanFire;

	// Whether or not the gun is in automatic mode.
	UPROPERTY()
	bool bAutoIsOn;

	// Server Only, The instigator of damage events.
	UPROPERTY()
	AController* _internalInstigator;

	// Server Only, the type of damage the gun inflicts.
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageType> DamageType;

	// The sound effect to play when the gun fires.
	UPROPERTY(Replicated, EditAnywhere)
	TSoftObjectPtr<class USoundCue> FiringSFX;

	// The muzzle flash to play at the USceneComponent* Origin.
	UPROPERTY(Replicated, EditAnywhere)
	TSoftObjectPtr<class UParticleSystem> FiringFlash;

public:
	UGunComponent();

	// Called whenever a gun is attempted to be fired when out of ammo.
	UPROPERTY(BlueprintAssignable)
	FGunComponentEvent OnOutOfAmmo;

	// Called whenever a reload is attempted on a gun with already full ammo.
	UPROPERTY(BlueprintAssignable)
	FGunComponentEvent OnAlreadyMaxAmmo;

	// Server Only, Sets the origin object for the bullet to be fired from.
	UPROPERTY(Replicated)
	USceneComponent* OriginPoint;

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
	void GunFire(const FVector& ForwardVector, const TArray<AActor*>& IgnoredActors);

	// Server Only, Reloads ammunition. Returns how many bullets were added.
	UFUNCTION(BlueprintCallable)
	int32 Reload();

	// Returns whether or not a reload is possible.
	UFUNCTION(BlueprintPure)
	bool CanReload();

	// Server Only, The sound effect to play when the gun fires.
	UFUNCTION(BlueprintCallable)
	void SetSoundFX(TSoftObjectPtr<USoundCue> NewCue);

	// Server Only, The muzzle flash to play at the USceneComponent* Origin.
	UFUNCTION(BlueprintCallable)
	void SetMuzzleFX(TSoftObjectPtr<UParticleSystem> NewPS);

	// Server Only, Sets the origin object for the bullet to be fired from.
	UFUNCTION(BlueprintCallable)
	void SetOriginPoint(USceneComponent* NewOrigin);

	// Server Only, Sets how fast the gun can fire per second.
	UFUNCTION(BlueprintCallable)
	void SetFireRate(float NewFireRate);

	// Server Only, Sets how much the damage is inflicted per bullet.
	UFUNCTION(BlueprintCallable)
	void SetDamage(float NewDamage);

	// Server Only, Sets the damage type used by the gun.
	UFUNCTION(BlueprintCallable)
	void SetDamageType(TSubclassOf<UDamageType> NewDamageType);

	// Server Only, Sets the Instigator to resolve damage events.
	UFUNCTION(BlueprintCallable)
	void SetInstigator(AController* Instigator);

	// Begins Automatic Fire
	// Note: Automatic Mode is just for state tracking and book keeping. The caller needs to actually trigger the fire events.
	UFUNCTION(BlueprintCallable)
	void BeginAuto();

	// Ends Automatic Fire
	UFUNCTION(BlueprintCallable)
	void EndAuto();

	UFUNCTION(BlueprintPure)
	bool IsAutoActive();
};

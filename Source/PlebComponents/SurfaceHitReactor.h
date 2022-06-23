// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SurfaceHitReactor.generated.h"

/*
	This is designed to work with the Gun Component and therefore has it as a dependency.
	For the sake of convienence this breaks my design philosophy for the project but I may produce a component with a 
	Standalone API in the future.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLEBCOMPONENTS_API USurfaceHitReactor : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Actually responds to the hit events creating trail / impact particles and playing SFX as needed.
	UFUNCTION(BlueprintCallable)
	void ReactToHit(class UHitscanComponent* Comp, const TArray<FHitResult>& Hits);

	// Sound effect played if a surface is struck. Distinguishes between surface.
	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EPhysicalSurface>, TSoftObjectPtr<class USoundCue> > HitImpactSFX;

	// Trails that trace the hit path.
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UParticleSystem> HitTrailParticle;

	// Particles that play upon the impact normal. Distinguishes between surface.
	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EPhysicalSurface>, TSoftObjectPtr<class UParticleSystem> > HitImpactParticle;

public:	
	// Sets default values for this component's properties
	USurfaceHitReactor();

	// Binds the protected ReactToHit delegate to the HitscanComponent.
	UFUNCTION(BlueprintCallable)
	void Bind(class UHitscanComponent* Comp);
};

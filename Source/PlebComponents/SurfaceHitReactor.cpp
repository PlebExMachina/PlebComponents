// Fill out your copyright notice in the Description page of Project Settings.


#include "SurfaceHitReactor.h"
#include "HitscanComponent.h"
#include "GunComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"


void USurfaceHitReactor::ReactToHit(UHitscanComponent* Comp, const TArray<FHitResult>& Hits)
{
	static const TEnumAsByte<EPhysicalSurface> DefaultSurface = EPhysicalSurface::SurfaceType_Default;
	auto GunComp = Cast<UGunComponent>(Comp);

	for (const auto& Hit : Hits) {
		TEnumAsByte<EPhysicalSurface> HitSurface = UGameplayStatics::GetSurfaceType(Hit);
		// Impact only on blocking hits
		if (Hit.bBlockingHit) {
			auto SFX = HitImpactSFX.Find(HitSurface);
			if (!SFX) { SFX = HitImpactSFX.Find(DefaultSurface); }
			if (SFX) {
				if (SFX->LoadSynchronous()) {
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFX->Get(), Hit.ImpactPoint);
				}
			}

			auto Particle = HitImpactParticle.Find(HitSurface);
			if (!Particle) { Particle = HitImpactParticle.Find(DefaultSurface); }
			if (Particle) {
				if (Particle->LoadSynchronous()) {
					// Spawn particle pointing away from the impact point at the impact point.
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle->Get(), FTransform((Hit.ImpactPoint + Hit.ImpactNormal).Rotation(), Hit.ImpactPoint));
				}
			}
		}
		// Trails can occur on any hits.
		if (HitTrailParticle.LoadSynchronous() && GunComp) {
			auto TrailEmitter = UGameplayStatics::SpawnEmitterAttached(HitTrailParticle.Get(), GunComp->OriginPoint, NAME_None, GunComp->OriginPoint->GetComponentLocation(), GunComp->OriginPoint->GetComponentRotation(), EAttachLocation::KeepWorldPosition);
			if (TrailEmitter) {
				TrailEmitter->SetBeamTargetPoint(0, Hit.ImpactPoint, 0);
			}
		}
	}
}

// Sets default values for this component's properties
USurfaceHitReactor::USurfaceHitReactor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}

void USurfaceHitReactor::Bind(UHitscanComponent* Comp)
{
	Comp->OnHitscanHit.AddDynamic(this, &USurfaceHitReactor::ReactToHit);
}


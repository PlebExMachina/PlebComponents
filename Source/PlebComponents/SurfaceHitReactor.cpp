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
				auto SFX_Cue = SFX->LoadSynchronous();
				if (SFX_Cue) {
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFX_Cue, Hit.ImpactPoint);
				}
			}

			auto Particle = HitImpactParticle.Find(HitSurface);
			if (!Particle) { Particle = HitImpactParticle.Find(DefaultSurface); }
			if (Particle) {
				auto Particle_Emitter = Particle->LoadSynchronous();
				if (Particle_Emitter) {
					// Spawn particle pointing away from the impact point at the impact point.
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle_Emitter, FTransform((Hit.ImpactPoint + Hit.ImpactNormal).Rotation(), Hit.ImpactPoint));
				}
			}
		}
		// Trails can occur on any hits.
		auto Trail = HitTrailParticle.LoadSynchronous();
		if (Trail && GunComp) {
			auto TrailEmitter = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Trail, GunComp->OriginPoint->GetComponentTransform());
			if (TrailEmitter) {
				TrailEmitter->SetBeamSourcePoint(0, GunComp->OriginPoint->GetComponentLocation(), 0);
				TrailEmitter->SetBeamTargetPoint(0, Hit.ImpactPoint, 0);
				UE_LOG(LogTemp, Warning, TEXT("TRAIL TRACED"));
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("HIT REACTION DONE"));
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


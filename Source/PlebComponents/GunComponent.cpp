// Fill out your copyright notice in the Description page of Project Settings.


#include "GunComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"

namespace {
	auto IsServer = [](UObject* o) -> bool { return o->GetWorld()->GetAuthGameMode() != nullptr; };
	const float TrueMinimum = 50.f;
};

void UGunComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UGunComponent, OriginPoint);
	DOREPLIFETIME(UGunComponent, MinimumLens);
	DOREPLIFETIME(UGunComponent, MaximumLens);
	DOREPLIFETIME(UGunComponent, Recoil);
	DOREPLIFETIME(UGunComponent, Recovery);
	DOREPLIFETIME(UGunComponent, MaximumAmmunition);
	DOREPLIFETIME(UGunComponent, CurrentAmmunition);
	DOREPLIFETIME(UGunComponent, AmmoConsumption);

}

void UGunComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* TickFunction) {
	Super::TickComponent(DeltaTime, TickType, TickFunction);
	if (IsServer(this)) {
		if (bDebug && DebugOrigin) {
			DrawDebugSphere(
				GetWorld(),
				DebugForwardVector * LensDistance + DebugOrigin->GetComponentLocation(),
				LensRadius, 32, FColor(255, 0, 0), false, 3.f
			);
		}

		LensDistance = FMath::Clamp(LensDistance + Recovery * DeltaTime, MinimumLens, MaximumLens);
	}
}

void UGunComponent::PlayMuzzleFX(UHitscanComponent* Comp, const TArray<FHitResult>& Hits) {
	if (OriginPoint) {
		if (FiringSFX.LoadSynchronous()) {
			UGameplayStatics::PlaySoundAtLocation(this, FiringSFX.Get(), OriginPoint->GetComponentLocation());
		}

		if (FiringFlash.LoadSynchronous()) {
			UGameplayStatics::SpawnEmitterAttached(FiringFlash.Get(), OriginPoint, NAME_None, OriginPoint->GetComponentLocation(), OriginPoint->GetComponentRotation(), EAttachLocation::KeepWorldPosition);
		}
	}
}

void UGunComponent::BeginPlay()
{
	Super::BeginPlay();
	if (IsServer(this)) {
		CurrentAmmunition = MaximumAmmunition;
	}

	OnHitscanHit.AddDynamic(this, &UGunComponent::PlayMuzzleFX);
}

UGunComponent::UGunComponent()
{
	MinimumLens = TrueMinimum;
	MaximumLens = 2000.f;
	Recoil = 400.f;
	Recovery = 2000.f;
	LensRadius = 30.f;

	MaximumAmmunition = 10;
	AmmoConsumption = 2;

	OriginPoint = nullptr;

	DebugOrigin = nullptr;
	DebugForwardVector = FVector::ZeroVector;
}

void UGunComponent::SetMinimumLens(float NewLensSize)
{
	if (IsServer(this)) {
		MinimumLens = FMath::Clamp(NewLensSize,TrueMinimum,MaximumLens);
		LensDistance = FMath::Clamp(LensDistance, MinimumLens, MaximumLens);
	}
}

float UGunComponent::GetMinimumLens()
{
	return MinimumLens;
}

void UGunComponent::SetMaximumLens(float NewLensSize)
{
	if(IsServer(this)){
		MaximumLens = (NewLensSize < MinimumLens) ? MinimumLens : NewLensSize;
		LensDistance = FMath::Clamp(LensDistance, MinimumLens, MaximumLens);
	}
}

float UGunComponent::GetMaximumLens()
{
	return MaximumLens;
}

void UGunComponent::SetRecoil(float NewRecoil)
{
	if(IsServer(this)){
		Recoil = NewRecoil;
	}
}

float UGunComponent::GetRecoil()
{
	return Recoil;
}

void UGunComponent::SetRecovery(float NewRecovery)
{
	if (IsServer(this)) {
		Recovery = NewRecovery;
	}
}

float UGunComponent::GetRecovery()
{
	return Recovery;
}


void UGunComponent::GunFire(const FVector& ForwardVector, const TArray<AActor*>& IgnoredActors) {
	if (IsServer(this) && OriginPoint) {
		if (CurrentAmmunition != 0) {
			Fire(OriginPoint->GetComponentLocation(), ForwardVector * FiringDistance + OriginPoint->GetComponentLocation(), IgnoredActors);
			LensDistance = FMath::Clamp(LensDistance - Recoil, MinimumLens, MaximumLens);

			// Negative Max Ammo is Infinite
			if (MaximumAmmunition >= 0) {
				CurrentAmmunition = (CurrentAmmunition - AmmoConsumption) < 0 ? 0 : (CurrentAmmunition - AmmoConsumption);
			}

			if (bDebug) {
				DebugOrigin = OriginPoint;
				DebugForwardVector = ForwardVector;
				UE_LOG(LogTemp, Warning, TEXT("Bullets Remaining: %d"), CurrentAmmunition);
			}
		} else {
			OnOutOfAmmo.Broadcast(this);
		}
	}
}

int32 UGunComponent::Reload()
{
	int32 out = 0;
	if(IsServer(this)){
		out = MaximumAmmunition - CurrentAmmunition;
		CurrentAmmunition = MaximumAmmunition;
		if (bDebug) {
			UE_LOG(LogTemp, Warning, TEXT("Bullets Reloaded: %d"), out);
		}
		if (out == 0) {
			OnAlreadyMaxAmmo.Broadcast(this);
		}
	}
	return out;
}

bool UGunComponent::CanReload() {
	return (MaximumAmmunition - CurrentAmmunition) != 0;
}

void UGunComponent::SetMuzzleFX(TSoftObjectPtr<UParticleSystem> NewPS)
{
	if (IsServer(this)) {
		FiringFlash = NewPS;
	}
}

void UGunComponent::SetSoundFX(TSoftObjectPtr<USoundCue> NewCue)
{
	if (IsServer(this)) {
		FiringSFX = NewCue; 
	}
}

void UGunComponent::SetOriginPoint(USceneComponent* NewOrigin)
{
	if (IsServer(this)) {
		OriginPoint = NewOrigin;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "HitscanComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "PlebComponentsAPI.h"

namespace {
	auto IsServer = [](UObject* o) -> bool { return o->GetWorld()->GetAuthGameMode() != nullptr; };
};

void UHitscanComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHitscanComponent, bIsAiming);
	DOREPLIFETIME(UHitscanComponent, AimBonus);
	DOREPLIFETIME(UHitscanComponent, LensRadius);
	DOREPLIFETIME(UHitscanComponent, LensDistance);
	DOREPLIFETIME(UHitscanComponent, FiringDistance);
	DOREPLIFETIME(UHitscanComponent, FireCount);
}

// Sets default values for this component's properties
UHitscanComponent::UHitscanComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	LensRadius = 10.f;
	LensDistance = 350.f;
	FiringDistance = 20000.f;
	FireCount = 2;
	FiringChannel = ECollisionChannel::ECC_Visibility;
	// ...
}

void UHitscanComponent::Fire(const FVector& Origin, const FVector& Target, const TArray<AActor*>& IgnoredActors)
{
	if (GetWorld()->GetAuthGameMode() != nullptr) {
		// Get Location of the Lens
		float CorrectedLensDistance = bIsAiming ? LensDistance + AimBonus : LensDistance;
		FVector LensLocation = ((Target - Origin).GetSafeNormal() * CorrectedLensDistance) + Origin;
		TArray<FHitResult> Hits = {};

		FCollisionQueryParams LocalCollisionParams = PlebComponentsAPI::PlebDefaultQueryParams;
		for (const AActor* Actor : IgnoredActors) {
			LocalCollisionParams.AddIgnoredActor(Actor);
		}

		// For each shot fired (Shotgun Case)
		for (int i = 0; i < FireCount; ++i) {
			// Get random point in lens.
			FVector LensSample = PlebComponentsAPI::RandomPointInSphere(LensLocation, LensRadius);

			// Get point towards target
			FVector AdjustedTarget = ((LensSample - Origin).GetSafeNormal() * FiringDistance) + Origin;

			if (bDebug) {
				DrawDebugLine(GetWorld(), Origin, AdjustedTarget, FColor(0, 255, 0), false, 3.f);
			}

			Hits.Add(PlebComponentsAPI::LineTraceSingleByLength(GetOwner(), Origin, AdjustedTarget, FiringDistance, LocalCollisionParams));
		}

		BroadcastHitResults(Hits);
	}
}

void UHitscanComponent::BroadcastHitResults_Implementation(const TArray<FHitResult>& HitResults) {
	OnHitscanHit.Broadcast(this,HitResults);
}

void UHitscanComponent::BeginAim() {
	if (IsServer(this)) {
		bIsAiming = true;
	}
}

void UHitscanComponent::EndAim() {
	if (IsServer(this)) {
		bIsAiming = false;
	}
}

void UHitscanComponent::SetAimBonus(float Bonus) {
	if (IsServer(this)) {
		AimBonus = Bonus;
	}
}

bool UHitscanComponent::IsAiming() {
	return bIsAiming;
}
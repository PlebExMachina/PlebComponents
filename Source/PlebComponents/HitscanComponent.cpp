// Fill out your copyright notice in the Description page of Project Settings.


#include "HitscanComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"

void UHitscanComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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
	// ...
}

void UHitscanComponent::Fire(const FVector& Origin, const FVector& Target, const TArray<AActor*>& IgnoredActors)
{
	if (GetWorld()->GetAuthGameMode() != nullptr) {
		// Get Location of the Lens
		FVector LensLocation = ((Target - Origin).GetSafeNormal() * LensDistance) + Origin;
		TArray<FHitResult> Hits = {};

		FCollisionQueryParams LocalCollisionParams;
		LocalCollisionParams.AddIgnoredActor(GetOwner());

		for (const AActor* Actor : IgnoredActors) {
			LocalCollisionParams.AddIgnoredActor(Actor);
		}

		// For each shot fired (Shotgun Case)
		for (int i = 0; i < FireCount; ++i) {
			FHitResult Hit;
			// Get random point in lens.
			FVector LensSample = FVector(
				FMath::RandRange(LensLocation.X - LensRadius, LensLocation.X + LensRadius),
				FMath::RandRange(LensLocation.Y - LensRadius, LensLocation.Y + LensRadius),
				FMath::RandRange(LensLocation.Z - LensRadius, LensLocation.Z + LensRadius)
			);

			FVector AdjustedTarget = ((LensSample - Origin).GetSafeNormal() * FiringDistance) + Origin;

			GetWorld()->LineTraceSingleByChannel(Hit, Origin, AdjustedTarget, ECollisionChannel::ECC_WorldStatic, LocalCollisionParams);
			if (bDebug) {
				DrawDebugLine(GetWorld(), Origin, AdjustedTarget, FColor(0, 255, 0), false, 3.f);
			}

			Hits.Add(Hit);
		}

		BroadcastHitResults(Hits);
	}
}

void UHitscanComponent::BroadcastHitResults_Implementation(const TArray<FHitResult>& HitResults) {
	OnHitscanHit.Broadcast(this,HitResults);
}
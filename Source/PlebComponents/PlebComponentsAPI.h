// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * General API for Pleb Component module.
 */
class PLEBCOMPONENTS_API PlebComponentsAPI
{
public:
	static FCollisionQueryParams PlebDefaultQueryParams;

	// Spawns an actor with the minimal needed information.
	template<typename T>
	static T* SpawnActor(UObject* Ctx, TSubclassOf<T> Class, const FTransform& SpawnTransform) {
		return Ctx->GetWorld()->SpawnActor<T>(Class, SpawnTransform.GetLocation(), SpawnTransform.GetRotation().Rotator());
	}

	// Gets a component with the minimal needed information.
	template<typename T>
	static T* GetComponent(AActor* Owner, UClass* Class = T::StaticClass()) {
		return Cast<T>(Owner->GetComponentByClass(Class));
	}

	// Checks if a given object implements the given UInterface
	// The object/class relationship with this funcntion can be confusing. This just hides those details.
	template<typename TInterface>
	static bool ImplementsInterface(UObject* Obj) {
		return Obj->GetClass()->ImplementsInterface(TInterface::StaticClass());
	}

	// Checks if the actor has a controller.
	template<typename T = AController>
	static AController* GetController(AActor* Actor) {
		auto PawnOwner = Cast<APawn>(Actor);
		if (PawnOwner) {
			return Cast<T>(PawnOwner->GetController());
		}
		return nullptr;
	}

	// Performs a generic directed line trace with common settings.
	// The default line trace single is quite verbose so this is mostly to cut down on that.
	static FHitResult LineTraceSingleByLength(
		AActor* Owner,
		const FVector& Start,
		const FVector& Direction,
		float Length,
		const FCollisionQueryParams& CollisionQueryParams = PlebComponentsAPI::PlebDefaultQueryParams,
		ECollisionChannel CollisionChannel = ECollisionChannel::ECC_Visibility
	);

	// Samples a random point within a sphereical radius of the location.
	static FVector RandomPointInSphere(const FVector& Location ,float Radius);
};

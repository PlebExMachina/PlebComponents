// Fill out your copyright notice in the Description page of Project Settings.


#include "PlebComponentsAPI.h"

// Initialize static value via lambda.
FCollisionQueryParams PlebComponentsAPI::PlebDefaultQueryParams = []() -> FCollisionQueryParams {
	auto QueryParam = FCollisionQueryParams::DefaultQueryParam;
	QueryParam.bReturnPhysicalMaterial = true;
	return QueryParam;
}();


FHitResult PlebComponentsAPI::LineTraceSingleByLength(
	AActor* Owner,
	const FVector& Start,
	const FVector& Direction,
	float Length,
	const FCollisionQueryParams& CollisionQueryParams,
	ECollisionChannel CollisionChannel
)
{
	FHitResult OutHit;
	auto QueryParamCopy = FCollisionQueryParams(CollisionQueryParams);
	QueryParamCopy.AddIgnoredActor(Owner);
	Owner->GetWorld()->LineTraceSingleByChannel(OutHit, Start, Start + Direction * Length, CollisionChannel, QueryParamCopy);
	return OutHit;
}

FVector PlebComponentsAPI::RandomPointInSphere(const FVector& Location, float Radius) {
	return FVector(
		FMath::RandRange(Location.X - Radius, Location.X + Radius),
		FMath::RandRange(Location.Y - Radius, Location.Y + Radius),
		FMath::RandRange(Location.Z - Radius, Location.Z + Radius)
	);
}
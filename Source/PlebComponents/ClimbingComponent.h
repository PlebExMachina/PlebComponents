// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ClimbingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNovelClimbingEvent);

/*
 * A component for climbing against flat surfaces.
 * This is useful for things like ladders, less so for round surfaces.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLEBCOMPONENTS_API UClimbingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Whether or not a climbing event is in progress.
	UPROPERTY(Replicated, ReplicatedUsing = "OnRep_ClimbingStateChanged")
	bool _IsClimbing;

	// The default "Gravity Scale" of the Character Movement Component.
	UPROPERTY()
	float _StoredGravityScale;

	// The default "Orient Rotation To Movement" of the Character Movement Component
	UPROPERTY()
	bool _StoredOrientRotation;

	// The 'normal' rotation of the wall. 
	// This can be replaced with a more generalized approach in the future.
	UPROPERTY(Replicated)
	FRotator _InternalWallNormalRotation;

	// The character movement component that needs to be modified for climbing to work.
	UPROPERTY()
	class UCharacterMovementComponent* _CharacterMovement;

public:	
	// Called whenever a Climbing Event begins.
	// Climb functions will likely be called by some Actor in the world, this provides a cleaner way for the Owner to make changes.
	UPROPERTY(BlueprintAssignable)
	FNovelClimbingEvent OnClimbBegin;

	// Called whenever a Climbing Event ends.
	// Climb functions will likely be called by some Actor in the world, this provides a cleaner way for the Owner to make changes.
	UPROPERTY(BlueprintAssignable)
	FNovelClimbingEvent OnClimbEnd;

	// Sets default values for this component's properties
	UClimbingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Pulls the heavy lifting of updating the movement data so that climbing will be convincing.
	UFUNCTION()
	void OnRep_ClimbingStateChanged(bool Old);

	// Lazy Getter for Character Movement Component
	UFUNCTION()
	class UCharacterMovementComponent* GetCharacterMovement();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Server Only. Begins the climbing event.
	UFUNCTION(BlueprintCallable)
	void BeginClimb(UPARAM(ref) const FRotator& WallNormalRotation);

	// Server only. Ends the climbing event.
	UFUNCTION(BlueprintCallable)
	void EndClimb();

	// Whether or not a climbing event is currently in progress.
	UFUNCTION(BlueprintPure)
	bool IsClimbing();

	// Checks if the actor is approximately 'touching the ground'
	// Useful for terminating a climbing event.
	UFUNCTION(BlueprintCallable)
	bool NearGround(UPARAM(ref) const FVector& ReferenceLocation, UPARAM(ref) const FVector& DownVector);
};

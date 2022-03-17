// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LedgeGrabbingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnLedgeGrab, ULedgeGrabbingComponent*, LedgeGrabComponent, const FHitResult&, ForwardHitLeft, const FHitResult&, ForwardHitRight, const FHitResult&, DownwardHitLeft, const FHitResult&, DownwardHitRight);

/*
* Provides an API / Detection for ledge grabbing.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLEBCOMPONENTS_API ULedgeGrabbingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Whether or not the Ledge Grab check is enabled. (Automatically set to true upon touching the ground.)
	UPROPERTY()
	bool _CheckEnabled;

	// Caller defined behavior. Mostly used for Control / Animation information.
	UPROPERTY(Replicated)
	bool _IsGrabbingLedge;

	// Represents how "wide" the character's grabbing ability is. (Should be the same as the character hand width when hanging)
	UPROPERTY(Replicated, EditAnywhere)
	float GrabRadius;

	// The owner's Character Movement Component
	UPROPERTY()
	class UCharacterMovementComponent* _CharacterMovement;


	UPROPERTY()
	class UMeshComponent* _Mesh;

	// Whether or not to draw the traces in the world. (Only has meaning for BP)
	UPROPERTY(EditAnywhere)
	bool DebugTrace;

	// The specific mesh class to look for. Determines the Mesh Class found by the lazy getter.
	// Assigning to this will potentially stop mattering as soon as the first tick.
	// This should be used only for initialization in C++. 
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMeshComponent> MeshClass;

	// Represents the "highest position" that a ledge can be grabbed.
	UPROPERTY(EditAnywhere)
	FName HighSocket;

	// Represents the "lowest position" that a ledge can be grabbed.
	UPROPERTY(EditAnywhere)
	FName LowSocket;

	// How far out from the actor to check for a ledge.
	UPROPERTY(EditAnywhere)
	float CheckLength;

public:	
	// Fires off when the character should be able to grab the ledge.
	UPROPERTY(BlueprintAssignable)
	FOnLedgeGrab OnLedgeGrab;

	// Sets default values for this component's properties
	ULedgeGrabbingComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Set whether or not the Ledge Grab check is enabled. (Only means anything in mid-air.) 
	UFUNCTION(BlueprintCallable)
	void SetCheckEnabled(bool NewEnabled);

	// Sets the mesh class. (Only matters for initialization.)
	UFUNCTION(BlueprintCallable)
	void SetMeshClass(TSubclassOf<class UMeshComponent> New);

	// Represents the "highest position" that a ledge can be grabbed.
	UFUNCTION(BlueprintCallable)
	void SetHighSocket(FName New);

	// Represents the "lowest position" that a ledge can be grabbed.
	UFUNCTION(BlueprintCallable)
	void SetLowSocket(FName New);

	// Represents how "wide" the character's grabbing ability is. (Should be the same as the character hand width when hanging)
	UFUNCTION(BlueprintCallable)
	void SetGrabRadius(float New);

	// How far out from the actor to check for a ledge.
	UFUNCTION(BlueprintCallable)
	void SetCheckLength(float New);

	// Snaps the actor position / rotation toward the ledge. SnapToLength provides a downard offset from the top of the ledge to "where the character should be"
	// It will also set character movement value to "Flying" so that it wont be affected by gravity.
	UFUNCTION(BlueprintCallable)
	void SnapToLedge(const FHitResult& ForwardHitLeft, const FHitResult& ForwardHitRight, const FHitResult& DownwardHitLeft, const FHitResult& DownwardHitRight, float ZOffset = 0.f, float NormalOffset = 0.f);

	// Set whether the ledge is grabbed (Must be set with this function, it is not tracked automatically.)
	UFUNCTION(BlueprintCallable)
	void SetIsGrabbingLedge(bool New);

protected:
	virtual void BeginPlay() override;

	// (Lazily) gets the owner's character movement component.
	UFUNCTION(BlueprintPure)
	class UCharacterMovementComponent* GetCharacterMovement();

	// (Lazily) gets the owner's mesh component.
	UFUNCTION(BlueprintPure)
	class UMeshComponent* GetMesh();

	// Get whether the ledge is being grabbed.
	UFUNCTION(BlueprintPure)
	bool GetIsGrabbingLedge();

	// Represents how "wide" the character's grabbing ability is. (Should be the same as the character hand width when hanging)
	UFUNCTION(BlueprintPure)
	float GetGrabRadius();
};

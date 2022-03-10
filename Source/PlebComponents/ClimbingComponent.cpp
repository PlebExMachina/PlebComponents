// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

namespace {
	auto IsServer = [](UObject* o) -> bool { return nullptr != o->GetWorld()->GetAuthGameMode(); };
};

// Sets default values for this component's properties
UClimbingComponent::UClimbingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	_CharacterMovement = nullptr;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetIsReplicatedByDefault(true);
	// ...
}


void UClimbingComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UClimbingComponent, _IsClimbing);
	DOREPLIFETIME(UClimbingComponent, _InternalWallNormalRotation);
}


// Called when the game starts
void UClimbingComponent::BeginPlay()
{
	Super::BeginPlay();
	// Get Default Character Movement data
	if (GetCharacterMovement()) {
		_StoredGravityScale = GetCharacterMovement()->GravityScale;
		_StoredOrientRotation = GetCharacterMovement()->bOrientRotationToMovement;
	}
	// ...
	
}


// Called every frame
void UClimbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// Track Actor Rotation towards the wall.
	if (GetCharacterMovement()) {
		GetOwner()->SetActorRotation(FMath::RInterpTo(GetOwner()->GetActorRotation(), _InternalWallNormalRotation, DeltaTime, 10.f));
		// If wall is fully tracked then end tick.
		if (GetOwner()->GetActorRotation().Equals(_InternalWallNormalRotation)) {
			SetComponentTickEnabled(false);
		}
	}
	// ...
}

UCharacterMovementComponent* UClimbingComponent::GetCharacterMovement() {
	// Lazy Get
	if (!_CharacterMovement) {
		_CharacterMovement = Cast<UCharacterMovementComponent>(GetOwner()->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	}
	return _CharacterMovement;
}

void UClimbingComponent::OnRep_ClimbingStateChanged(bool Old) {
	if (_IsClimbing) {
		// Begin Climbing, "Fly", kill velocity, ignore gravity.
		if (GetCharacterMovement()) {
			GetCharacterMovement()->bOrientRotationToMovement = false;
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			GetCharacterMovement()->GravityScale = 0.f;
			GetCharacterMovement()->RequestedVelocity = FVector(0.f, 0.f, 0.f);
			OnClimbBegin.Broadcast();
			SetComponentTickEnabled(true);
		}
	}
	else {
		// End Climbing, "Walk, restore gravity.
		GetCharacterMovement()->bOrientRotationToMovement = _StoredOrientRotation;
		GetCharacterMovement()->GravityScale = _StoredGravityScale;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		OnClimbEnd.Broadcast();
		SetComponentTickEnabled(false);
	}
}

void UClimbingComponent::BeginClimb(const FRotator& WallNormalRotation) {
	if (IsServer(this)) {
		_IsClimbing = true;
		_InternalWallNormalRotation = WallNormalRotation;
		OnRep_ClimbingStateChanged(false);
	}
}

void UClimbingComponent::EndClimb() {
	if (IsServer(this)) {
		_IsClimbing = false;
		OnRep_ClimbingStateChanged(true);
	}
}

bool UClimbingComponent::IsClimbing() {
	return _IsClimbing;
}

bool UClimbingComponent::NearGround(const FVector& ReferenceLocation, const FVector& DownVector) {
	// Raytrace downward from reference location (such as ball of foot)
	FHitResult OutHit;
	FCollisionObjectQueryParams ObjectQueryParams = FCollisionObjectQueryParams::DefaultObjectQueryParam;
	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OutHit, ReferenceLocation, ReferenceLocation + DownVector * 10.f, ObjectQueryParams, CollisionQueryParams);
	return OutHit.bBlockingHit;
}
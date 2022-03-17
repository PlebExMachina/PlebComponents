// Fill out your copyright notice in the Description page of Project Settings.


#include "LedgeGrabbingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/MeshComponent.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"

namespace {
	auto IsServer = [](UObject* o) -> bool { return nullptr != o->GetWorld()->GetAuthGameMode(); };
};

void ULedgeGrabbingComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ULedgeGrabbingComponent,_IsGrabbingLedge);
	DOREPLIFETIME(ULedgeGrabbingComponent, GrabRadius);
}

// Sets default values for this component's properties
ULedgeGrabbingComponent::ULedgeGrabbingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.TickInterval = 0.25f;				// Tick Interval should be high to reduce trace load. It shouldn't look too unnatrual anyways.
	SetIsReplicatedByDefault(true);
	_CheckEnabled = true;
	_CharacterMovement = nullptr;
	_Mesh = nullptr;
	DebugTrace = false;
	MeshClass = UMeshComponent::StaticClass();
	HighSocket = "LedgeHigh";
	LowSocket = "LedgeLow";
	CheckLength = 100.f;
	GrabRadius = 40.f;
}

// Called every frame
void ULedgeGrabbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetWorld()->GetAuthGameMode() && GetCharacterMovement()) {
		if (GetCharacterMovement()->IsFalling() && (GetCharacterMovement()->Velocity.Z < 0.f)) {
			if (_CheckEnabled && GetMesh()) {
				FHitResult ForwardTraceResultLeft, ForwardTraceResultRight;
				FHitResult DownwardTraceResultLeft, DownwardTraceResultRight;

				// Only check for static geometry.
				FCollisionObjectQueryParams ObjectQueryParams = FCollisionObjectQueryParams::DefaultObjectQueryParam;
				ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);

				// Ignore Owner
				FCollisionQueryParams QueryCollisionParams = FCollisionQueryParams::DefaultQueryParam;
				TArray<AActor*> IgnoredActors = { GetOwner() };
				QueryCollisionParams.AddIgnoredActors(IgnoredActors);

				FVector DistanceOffset = GetOwner()->GetActorForwardVector()*CheckLength;
				FVector High = GetMesh()->GetSocketLocation(HighSocket);
				FVector Low = GetMesh()->GetSocketLocation(LowSocket);
				FVector HighFar = High + DistanceOffset;
				FVector LowFar = Low + DistanceOffset;
				FVector RightOffset = GetOwner()->GetActorRightVector()*GrabRadius;
				FVector LeftOffset = RightOffset * -1;
				// Trace Forward checking if anything can be potentially grabbable.
				GetWorld()->LineTraceSingleByObjectType(
					ForwardTraceResultLeft,
					Low + LeftOffset,
					LowFar + LeftOffset,
					ObjectQueryParams,
					QueryCollisionParams
				);

				GetWorld()->LineTraceSingleByObjectType(
					ForwardTraceResultRight,
					Low + RightOffset,
					LowFar + RightOffset,
					ObjectQueryParams,
					QueryCollisionParams
				);

				if (DebugTrace) {
					DrawDebugLine(
						GetWorld(),
						Low + LeftOffset,
						LowFar + LeftOffset,
						FColor(255, 0, 0),
						false,
						0.25,
						0,
						12.3
					);
					DrawDebugLine(
						GetWorld(),
						Low + RightOffset,
						LowFar + RightOffset,
						FColor(255, 0, 0),
						false,
						0.25,
						0,
						12.3
					);
				}

				// If successful move towards downward trace to see if the object's ledge is available.
				if (ForwardTraceResultLeft.bBlockingHit && ForwardTraceResultRight.bBlockingHit) {
					GetWorld()->LineTraceSingleByObjectType(
						DownwardTraceResultLeft,
						HighFar + LeftOffset,
						LowFar + LeftOffset,
						ObjectQueryParams,
						QueryCollisionParams
					);		


					GetWorld()->LineTraceSingleByObjectType(
						DownwardTraceResultRight,
						HighFar + RightOffset,
						LowFar + RightOffset,
						ObjectQueryParams,
						QueryCollisionParams
					);

					if (DebugTrace) {
						DrawDebugLine(
							GetWorld(),
							HighFar + LeftOffset,
							LowFar + LeftOffset,
							FColor(255, 0, 0),
							false,
							0.25,
							0,
							12.3
						);
						DrawDebugLine(
							GetWorld(),
							HighFar + RightOffset,
							LowFar + RightOffset,
							FColor(255, 0, 0),
							false,
							0.25,
							0,
							12.3
						);
					}

					// If downward trace is successful then make sure they actually hit the same object.
					if (DownwardTraceResultLeft.bBlockingHit && DownwardTraceResultRight.bBlockingHit) {
						//DownwardTraceResult.GetActor() && (DownwardTraceResult.GetActor() == ForwardTraceResult.GetActor())
						auto FinalCheck = [](const TArray<FHitResult>& Hits) -> bool {
							for (const auto& Hit : Hits) {
								if (!Hit.GetActor()) {
									return false;
								}
							}
							return true; 
						};
						if (FinalCheck({ DownwardTraceResultLeft, DownwardTraceResultRight, ForwardTraceResultLeft, ForwardTraceResultRight })) {
							SetCheckEnabled(false);
							OnLedgeGrab.Broadcast(this, ForwardTraceResultLeft, ForwardTraceResultRight, DownwardTraceResultLeft, DownwardTraceResultRight);
						}
					}
				}
			}
		}
		else {
			SetCheckEnabled(true);
		}
	}
	// ...
}

void ULedgeGrabbingComponent::BeginPlay() {
	Super::BeginPlay();
	if (IsServer(this)) {
		SetComponentTickEnabled(true);
	}
}

void ULedgeGrabbingComponent::SetCheckEnabled(bool NewEnabled) {
	if (IsServer(this)) {
		_CheckEnabled = NewEnabled;
	}
}

UCharacterMovementComponent* ULedgeGrabbingComponent::GetCharacterMovement() {
	if (!_CharacterMovement) {
		_CharacterMovement = Cast<UCharacterMovementComponent>(GetOwner()->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	}
	return _CharacterMovement;
}

UMeshComponent* ULedgeGrabbingComponent::GetMesh()
{
	if (!_Mesh) {
		_Mesh = Cast<UMeshComponent>(GetOwner()->GetComponentByClass(MeshClass));
	}
	return _Mesh;
}

void ULedgeGrabbingComponent::SetMeshClass(TSubclassOf<UMeshComponent> New) {
	if (IsServer(this)) {
		MeshClass = New;
	}
}

void ULedgeGrabbingComponent::SetHighSocket(FName New) {
	if (IsServer(this)) {
		HighSocket = New;
	}
}

void ULedgeGrabbingComponent::SetLowSocket(FName New) {
	if (IsServer(this)) {
		LowSocket = New;
	}
}

void ULedgeGrabbingComponent::SetCheckLength(float New) {
	if (IsServer(this)) {
		CheckLength = New;
	}
}

void ULedgeGrabbingComponent::SnapToLedge(const FHitResult& ForwardHitLeft, const FHitResult& ForwardHitRight, const FHitResult& DownwardHitLeft, const FHitResult& DownwardHitRight, float ZOffset, float NormalOffset)
{
	// Disable Movement / Gravity
	if (GetCharacterMovement()) {
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
	}

	float HalfNormalOffset = NormalOffset / 2.f;

	// Get Midpoints for grab location.
	auto VectorMidpoint = [](const FVector& A, const FVector& B) -> FVector {
		return (A - B) / 2 + B;
	};
	FVector ForwardImpactMidPoint = VectorMidpoint(ForwardHitRight.ImpactPoint, ForwardHitLeft.ImpactPoint);
	FVector DownwardImpactMidPoint = VectorMidpoint(DownwardHitLeft.ImpactPoint, DownwardHitRight.ImpactPoint);

	// Compute Offset using half of each impact normal.
	FVector NewActorLocation = ForwardImpactMidPoint + ForwardHitLeft.ImpactNormal*HalfNormalOffset + ForwardHitRight.ImpactNormal*HalfNormalOffset;

	// Offset grabbing location (inconsistencies in height could easily be solved with IK since both surfaces are "in range")
	NewActorLocation.Z = DownwardImpactMidPoint.Z - ZOffset;

	// Get inbetween rotation of each surface to handle literal corner cases.
	FRotator NewActorRotation = ((ForwardHitLeft.ImpactNormal * -1) + (ForwardHitRight.ImpactNormal * -1)).Rotation();

	GetOwner()->SetActorLocation(NewActorLocation, false, nullptr, ETeleportType::ResetPhysics);
	GetOwner()->SetActorRotation(NewActorRotation);
}

void ULedgeGrabbingComponent::SetIsGrabbingLedge(bool New) {
	if (IsServer(this)) {
		_IsGrabbingLedge = New;
	}
}

bool ULedgeGrabbingComponent::GetIsGrabbingLedge() {
	return _IsGrabbingLedge;
}

void ULedgeGrabbingComponent::SetGrabRadius(float New) {
	if (IsServer(this)) {
		GrabRadius = New;
	}
}

float ULedgeGrabbingComponent::GetGrabRadius() {
	return GrabRadius;
}
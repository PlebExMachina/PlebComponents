// Fill out your copyright notice in the Description page of Project Settings.


#include "SnapToComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

namespace {
	auto IsServer = [](UObject* o) -> bool {
		return o->GetWorld()->GetAuthGameMode() != nullptr;
	};
};

void USnapToComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USnapToComponent, _TranslateActor);
	DOREPLIFETIME(USnapToComponent, _TranslateComponent);
	DOREPLIFETIME(USnapToComponent, _TranslateLocation);
	DOREPLIFETIME(USnapToComponent, _RotationActorRef);
	DOREPLIFETIME(USnapToComponent, _RotationComponentRef);
	DOREPLIFETIME(USnapToComponent, _RotationLocationRef);
	DOREPLIFETIME(USnapToComponent, bRotatePitch);
	DOREPLIFETIME(USnapToComponent, bRotateYaw);
	DOREPLIFETIME(USnapToComponent, bTranslateX);
	DOREPLIFETIME(USnapToComponent, bTranslateY);
	DOREPLIFETIME(USnapToComponent, bTranslateZ);
	DOREPLIFETIME(USnapToComponent, fRotateSpeed);
	DOREPLIFETIME(USnapToComponent, fTranslateSpeed);
	DOREPLIFETIME(USnapToComponent, TranslateMode);
	DOREPLIFETIME(USnapToComponent, RotateMode);

	// This is used so that replication can control ticking for late joins and etc.
	DOREPLIFETIME(USnapToComponent, _bIsTicking);
}

// Sets default values for this component's properties
USnapToComponent::USnapToComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetIsReplicatedByDefault(true);

	// Defaults
	bRotatePitch = false;
	bRotateYaw = true;
	bTranslateX = true;
	bTranslateY = true;
	bTranslateZ = false;
	fRotateSpeed = 600.f;
	fTranslateSpeed = 200.f;
	TranslateMode = ESnapMode::Actor;
	RotateMode = ESnapMode::Actor;

	_TranslateActor = nullptr;
	_TranslateComponent = nullptr;
	_TranslateLocation = FVector::ZeroVector;

	_RotationActorRef = nullptr;
	_RotationComponentRef = nullptr;
	_RotationLocationRef = FVector::ZeroVector;

	_bIsTicking = false;
}


const FVector USnapToComponent::_GetSnapToLocation(AActor* Actor)
{
	FVector FocusLocation = FVector::ZeroVector;
	switch (TranslateMode) {
	case ESnapMode::Actor:
		FocusLocation = _TranslateActor != nullptr ? _TranslateActor->GetActorLocation() : Actor->GetActorLocation();
		break;
	case ESnapMode::Component:
		FocusLocation = _TranslateComponent != nullptr ? _TranslateComponent->GetComponentLocation() : Actor->GetActorLocation();
		break;
	case ESnapMode::Manual:
		FocusLocation = _TranslateLocation;
		break;
	}

	// If ignoring then set actor current value as final value.
	if (!bTranslateX) {
		FocusLocation.X = Actor->GetActorLocation().X;
	}
	if (!bTranslateY) {
		FocusLocation.Y = Actor->GetActorLocation().Y;
	}
	if (!bTranslateZ) {
		FocusLocation.Z = Actor->GetActorLocation().Z;
	}

	return FocusLocation;
}

const FVector USnapToComponent::_GetSnapToRotationRef(AActor* Actor)
{
	FVector FocusLocation = FVector::ZeroVector;
	switch (RotateMode) {
	case ESnapMode::Actor:
		FocusLocation = _RotationActorRef != nullptr ? _RotationActorRef->GetActorLocation() : Actor->GetActorLocation();
		break;
	case ESnapMode::Component:
		FocusLocation = _RotationComponentRef != nullptr ? _RotationComponentRef->GetComponentLocation() : Actor->GetActorLocation();
		break;
	case ESnapMode::Manual:
		FocusLocation = _RotationLocationRef;
		break;
	}

	return FocusLocation;
}

const FRotator USnapToComponent::_GetSnapToRotation(AActor* Actor)
{
	FRotator FocusRotation = UKismetMathLibrary::FindLookAtRotation(Actor->GetActorLocation(), _GetSnapToRotationRef(Actor));

	// If ignoring then set actor current value as final value.
	if (!bRotatePitch) {
		FocusRotation.Pitch = Actor->GetActorRotation().Pitch;
	}
	if (!bRotateYaw) {
		FocusRotation.Yaw = Actor->GetActorRotation().Yaw;
	}
	return FocusRotation;
}

void USnapToComponent::OnRep_SetTick()
{
	SetComponentTickEnabled(_bIsTicking);
}

// Called every frame
void USnapToComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bool ShouldRotate = bRotatePitch || bRotateYaw;
	bool ShouldTranslate = bTranslateX || bTranslateY || bTranslateZ;

	auto Actor = GetOwner();
	bool InterruptionOccured = false;

	// If disabled then treat it as if it already succeeded.
	bool RotationFinished = !ShouldRotate || false;
	bool TranslationFinished = !ShouldTranslate || false;

	// interpolate rotation
	if (ShouldRotate) {
		FRotator InterpolatedRotator = UKismetMathLibrary::RInterpTo_Constant(Actor->GetActorRotation(), _GetSnapToRotation(Actor), DeltaTime, fRotateSpeed);
		InterruptionOccured |= ! Actor->SetActorRotation(InterpolatedRotator);
		RotationFinished = _GetSnapToRotation(Actor).Equals(Actor->GetActorRotation(),0.01f);
	}

	// interpolate location
	if (ShouldTranslate) {
		FVector InterpolatedVector = UKismetMathLibrary::VInterpTo_Constant(Actor->GetActorLocation(), _GetSnapToLocation(Actor), DeltaTime, fTranslateSpeed);
		InterruptionOccured |= ! Actor->SetActorLocation(InterpolatedVector, true, nullptr, ETeleportType::TeleportPhysics);
		TranslationFinished = _GetSnapToLocation(Actor).Equals(Actor->GetActorLocation(), 0.01f);
	}

	// Only track actors for events on server.
	if (IsServer(this)) {
		if (InterruptionOccured) {
			OnFocusInterrupted.Broadcast(this);
		}
		if (!InterruptionOccured && RotationFinished && TranslationFinished) {
			OnFocusFinished.Broadcast(this);
		}
	}
}

void USnapToComponent::SetTranslateActor(AActor* FocusActor)
{
	if (IsServer(this)) {
		_TranslateActor = FocusActor;
	}
}

void USnapToComponent::SetTranslateComponent(USceneComponent* FocusComponent)
{
	if (IsServer(this)) {
		_TranslateComponent = FocusComponent;
	}
}

void USnapToComponent::SetTranslateLocation(const FVector& FocusLocation)
{
	if (IsServer(this)) {
		_TranslateLocation = FocusLocation;
	}
}

void USnapToComponent::SetRotationActorRef(AActor* FocusActor)
{
	if (IsServer(this)) {
		_RotationActorRef = FocusActor;
	}
}

void USnapToComponent::SetRotationComponentRef(USceneComponent* FocusComponent)
{
	if (IsServer(this)) {
		_RotationComponentRef = FocusComponent;
	}
}

void USnapToComponent::SetRotationLocationRef(const FVector& FocusLocation)
{
	if (IsServer(this)) {
		_RotationLocationRef = FocusLocation;
	}
}

void USnapToComponent::BeginFocus()
{
	if (IsServer(this) && !_bIsTicking) {
		_bIsTicking = true;
		OnFocusStarted.Broadcast(this);
		OnRep_SetTick();
	}
}

void USnapToComponent::EndFocus()
{
	if (IsServer(this) && _bIsTicking) {
		_bIsTicking = false;
		OnFocusInterrupted.Broadcast(this);
		OnRep_SetTick();
	}
}


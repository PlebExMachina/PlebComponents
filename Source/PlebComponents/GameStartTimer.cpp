// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartTimer.h"
#include "Net/UnrealNetwork.h"

void UGameStartTimer::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UGameStartTimer, _internalGameAlreadyStarted);
	DOREPLIFETIME(UGameStartTimer, _internalRemainingTime);
}

void UGameStartTimer::BroadcastGameStart_Implementation()
{
	OnGameStart.Broadcast();
}

// Sets default values for this component's properties
UGameStartTimer::UGameStartTimer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(false);
	_internalGameAlreadyStarted = false;
	_internalRemainingTime = 0.f;
	SetIsReplicatedByDefault(true);
	// ...
}

float UGameStartTimer::GetRemainingTime() {
	return _internalRemainingTime;
}

bool UGameStartTimer::GetGameAlreadyStarted() {
	return _internalGameAlreadyStarted;
}

void UGameStartTimer::StopTimer(bool StartGame) {
	// Stop timer. If game hasn't already started then broadcast that the game is beginning.
	if (GetOwner()->GetWorld()->GetAuthGameMode()) {
		SetComponentTickEnabled(false);
		if (StartGame && !_internalGameAlreadyStarted) {
			_internalGameAlreadyStarted = true;
			BroadcastGameStart();
		}
	}
}

void UGameStartTimer::StartTimer(float StartTime) {
	// Begins timer initializing it to the StartTime.
	if (GetOwner()->GetWorld()->GetAuthGameMode()) {
		_internalRemainingTime = StartTime;
		SetComponentTickEnabled(true);
	}
}

// Called every frame
void UGameStartTimer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// Tick Timer
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetOwner()->GetWorld()->GetAuthGameMode()) {
		_internalRemainingTime = FMath::Max(0.f, _internalRemainingTime - DeltaTime);

		// Time Out, Begin Game
		if (_internalRemainingTime == 0.f) {
			StopTimer(true);
		}
	}
}


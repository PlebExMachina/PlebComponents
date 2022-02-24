// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStartTimer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStartDelegate);

/*
* Helps with the start timer in an online multiplayer game such as if there is a ready screen / character select.
* This component is inteded to be attached to AGameState.
*
* Things like 'number of players' doesn't need to be tracked because what is tracking that can just short circuit
* the game start with "Stop Timer"
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLEBCOMPONENTS_API UGameStartTimer : public UActorComponent
{
	GENERATED_BODY()


protected:

	/*
	* Alerts interested parties ie: Late joins / the server if the game is 'already in progress'
	*/
	UPROPERTY(Replicated)
	bool _internalGameAlreadyStarted;

	/*
	* How much time is left before the game actually starts. Useful for UI so it is exposed.
	*/
	UPROPERTY(Replicated)
	float _internalRemainingTime;

	/*
	* Internal RPC to broadcast when the game has started for better or worse.
	*/
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastGameStart();

public:
	/*
	* Delegate is called when the game starts.
	*/
	UPROPERTY(BlueprintAssignable)
	FOnGameStartDelegate OnGameStart;

	// Sets default values for this component's properties
	UGameStartTimer();

	/*
	* How much time is left before the game actually starts. Useful for UI so it is exposed.
	*/
	UFUNCTION(BlueprintPure)
	float GetRemainingTime();

	/*
	* Alerts interested parties ie: Late joins / the server if the game is 'already in progress'
	*/
	UFUNCTION(BlueprintPure)
	bool GetGameAlreadyStarted();


	/*
	* Begins the start of match timer as well as initializes the start time.
	*
	* Only has an effect if called on the server.
	*/
	UFUNCTION(BlueprintCallable)
	void StartTimer(float StartTime);

	/*
	* Stops the start of match timer and optionally starts the game regardless of the current state.
	*
	* Only has an effect if called on the server.
	*/
	UFUNCTION(BlueprintCallable)
	void StopTimer(bool StartGame = true);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

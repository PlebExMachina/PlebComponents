// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ControllerLogin.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginDelegate, APlayerController*, Controller);

/*
* Component to be attatched to a PlayerController, it lets any observers know if / when it will be safe to
* call Multicast RPCs.
* 
* This was created because at the time of AGameMode::PostLogin Multicast RPC will not replicate to the client.
* 
* !! -- It is probably preferable to use replication and On_Rep functions wherever this component could be used.
*/
UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLEBCOMPONENTS_API UControllerLogin : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Whether or not the controller has finished logging in.
	UPROPERTY()
	bool LoggedIn;

	// Manages the TryLogin Timer.
	UPROPERTY()
	FTimerHandle RetryHandle;

public:
	// Alerts all listeners when the controller finishes logging in.
	UPROPERTY(BlueprintAssignable)
	FOnLoginDelegate OnLogin;

	// Returns whether or not the controller has already logged in. Should be called before binding to OnLogin.
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsLoggedIn() { return LoggedIn; }

	// Sets default values for this component's properties
	UControllerLogin();

protected:

	// Called when the game starts
	UFUNCTION()
	virtual void BeginPlay() override;

	// Pings server to see if Client -> Server relationship has been established.
	UFUNCTION(Server, Reliable)
	void TryLogin();

	// Pings Client / Server that Multicast-Safe relationship has been made.
	UFUNCTION(NetMulticast, Reliable)
	void ResolveLogin();
};

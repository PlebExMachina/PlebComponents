// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GenericState.generated.h"

/*
* Sometimes little pieces of information need to be updated across the network.
* This can be little tags or single variable sets.
*
* This component provides a generic container for UE4 Primitive to have their changes propagate across a network.
* This component utilizes RPCs and therefore shouldn't be used for persistant state.
* 
* It's useful for flagging state for the server but not so much for actually sharing that state with other clients.
*
* This doesn't have a 'Preferred Actor' and can be attatched to any. 
* It loses a lot of it's meaning on GameMode however.
*
* WARNING: This component can possibly introduce security risks. Do not use it for anything that needs to be validated.
* A typical use case is it may deliver state information about what a player may "want" to do. 
* It's still up to the Server to make sure there's no Foul Play.
* 
* !! - I do not recommend using this component at all but having a dynamic catch-all container can be tempting.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLEBCOMPONENTS_API UGenericState : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TMap<FName, int32> IntStore;

	UPROPERTY()
	TMap<FName, FString> StringStore;

	UPROPERTY()
	TMap<FName, FName> NameStore;

	UPROPERTY()
	TMap<FName, bool> BoolStore;

public:	
	// Sets default values for this component's properties
	UGenericState();
	
	UFUNCTION(Server, Reliable)
	void RequestWriteInt(FName Key, int32 Value);

	UFUNCTION(Server, Reliable)
	void RequestWriteString(FName Key, const FString& Value);

	UFUNCTION(Server, Reliable)
	void RequestWriteName(FName Key, FName Value);

	UFUNCTION(Server, Reliable)
	void RequestWriteBool(FName Key, bool Value);

	UFUNCTION(NetMulticast, Reliable)
	void WriteInt(FName Key, int32 Value);

	UFUNCTION(NetMulticast, Reliable)
	void WriteString(FName Key, const FString& Value);

	UFUNCTION(NetMulticast, Reliable)
	void WriteName(FName Key, FName Value);

	UFUNCTION(NetMulticast, Reliable)
	void WriteBool(FName Key, bool Value);

	UFUNCTION(BlueprintPure)
	int32 GetInt(FName Key);

	UFUNCTION(BlueprintPure)
	const FString& GetString(FName Key);

	UFUNCTION(BlueprintPure)
	FName GetName(FName Key);

	UFUNCTION(BlueprintPure)
	bool GetBool(FName Key);
};

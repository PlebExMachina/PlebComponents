// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputDelegator.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInputDelegate, FName, Input, UObject*, Ctx);

/*
* This provides a common container for listening to inputs for things such as UI.
* This is not neccessarily effecient but it is decentralized and should greatly simplify UI Logic.
*
* The Context object can be used for the listener to compare against so it doesn't respond to input that
* isn't intended for it.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLEBCOMPONENTS_API UInputDelegator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInputDelegator();

	UPROPERTY(BlueprintAssignable)
	FOnInputDelegate OnPress;

	UPROPERTY(BlueprintAssignable)
	FOnInputDelegate OnRelease;		
};

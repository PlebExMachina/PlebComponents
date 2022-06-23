// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Pool.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPool : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PLEBCOMPONENTS_API IPool
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Called when an object is first constructed and added to a pool.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Initialize(class UObjectPooler* Pooler);

	// Called when an object is added back into the pool (pseudo-deleted)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Release();
};

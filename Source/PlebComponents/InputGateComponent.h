// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputGateComponent.generated.h"

/*
* Meant to lock / unlock access to an interaction to give exclusive access to a single actor.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLEBCOMPONENTS_API UInputGateComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	APawn* _Interactor;

public:	
	// Sets default values for this component's properties
	UInputGateComponent();

	// Gets currently active interactor.
	UFUNCTION(BlueprintPure)
	APawn* GetInteractor();

	// Attempts to lock gate. Asserts that the interactor has the requisite components.
	// Disables Player Input
	UFUNCTION(BlueprintCallable)
	bool Lock(APawn* Interactor, const TArray<TSubclassOf<UActorComponent> >& RequiredComponents);

	// Unlocks gate and enabled player input.
	UFUNCTION(BlueprintCallable)
	void Unlock();
};

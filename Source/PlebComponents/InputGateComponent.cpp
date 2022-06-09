// Fill out your copyright notice in the Description page of Project Settings.


#include "InputGateComponent.h"
#include "AIController.h"

// Sets default values for this component's properties
UInputGateComponent::UInputGateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	_Interactor = nullptr;
}

APawn* UInputGateComponent::GetInteractor()
{
	return _Interactor;
}

bool UInputGateComponent::Lock(APawn* Interactor, const TArray<TSubclassOf<UActorComponent>>& RequiredComponents)
{
	// Server Only
	if (GetWorld()->GetAuthGameMode()) {
		// Assert Interactor Exists
		if (_Interactor == nullptr) {
			// Make sure it's player controlled.
			APlayerController* InteractorController = Interactor->GetController<APlayerController>();
			if (InteractorController) {
				// Make sure it has the needed components.
				for (auto CompClass : RequiredComponents) {
					if (Interactor->GetComponentByClass(CompClass) == nullptr) {
						return false;
					}
				}
				// Lock
				Interactor->DisableInput(InteractorController);
				_Interactor = Interactor;
				return true;
			}
		}
	}
	return false;
}

void UInputGateComponent::Unlock()
{
	// Server Only
	if (GetWorld()->GetAuthGameMode()) {
		// Unlock
		if (_Interactor) {
			auto InteractorController = _Interactor->GetController<APlayerController>();
			if (InteractorController) {
				_Interactor->EnableInput(InteractorController);
			}
			_Interactor = nullptr;
		}
	}
}


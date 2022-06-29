// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractorComponent.h"
#include "InterfaceInteractable.h"
#include "PlebComponentsAPI.h"

// Sets default values for this component's properties
UInteractorComponent::UInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractorComponent::_ServerInteract_Implementation(AController* Caller, UObject* Target)
{
	if (PlebComponentsAPI::ImplementsInterface<UInterfaceInteractable>(Target)) {
		IInterfaceInteractable::Execute_Interact(Target, Caller);
	}
}

void UInteractorComponent::TryInteract(UObject* Target)
{
	auto Controller = PlebComponentsAPI::GetController(GetOwner());
	if (Controller) {
		_ServerInteract(Controller, Target);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("UInteractorComponent::TryInteract -- No Controller"));
	}
}

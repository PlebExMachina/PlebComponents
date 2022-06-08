// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractorComponent.h"
#include "InterfaceInteractable.h"

// Sets default values for this component's properties
UInteractorComponent::UInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractorComponent::_ServerInteract_Implementation(AController* Caller, UObject* Target)
{
	if (Target->GetClass()->ImplementsInterface(UInterfaceInteractable::StaticClass())) {
		IInterfaceInteractable::Execute_Interact(Target, Caller);
	}
}

void UInteractorComponent::TryInteract(UObject* Target)
{
	auto PawnOwner = Cast<APawn>(GetOwner());
	if (PawnOwner) {
		auto Controller = PawnOwner->GetController();
		if (Controller) {
			_ServerInteract(Controller, Target);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("UInteractorComponent::TryInteract -- No Controller"));
		}
	}
}

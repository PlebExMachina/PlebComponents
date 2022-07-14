// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageReceiverComponent.h"
#include "Damagable.h"
#include "PlebComponentsAPI.h"

// Sets default values for this component's properties
UDamageReceiverComponent::UDamageReceiverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


void UDamageReceiverComponent::AnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (DamageType->GetClass()->ImplementsInterface(UDamagable::StaticClass())) {
		IDamagable::Execute_AnyDamage(DamageType, DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
	}
}

void UDamageReceiverComponent::PointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	if (DamageType->GetClass()->ImplementsInterface(UDamagable::StaticClass())) {
		IDamagable::Execute_PointDamage(DamageType, DamagedActor, Damage, InstigatedBy, HitLocation, FHitComponent, BoneName, ShotFromDirection, DamageType, DamageCauser);
	}
}

void UDamageReceiverComponent::RadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	if (DamageType->GetClass()->ImplementsInterface(UDamagable::StaticClass())) {
		IDamagable::Execute_RadialDamage(DamageType, DamagedActor, Damage, DamageType, Origin, HitInfo, InstigatedBy, DamageCauser);
	}
}

// Called when the game starts
void UDamageReceiverComponent::BeginPlay()
{
	Super::BeginPlay();
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UDamageReceiverComponent::AnyDamage);
	GetOwner()->OnTakePointDamage.AddDynamic(this, &UDamageReceiverComponent::PointDamage);
	GetOwner()->OnTakeRadialDamage.AddDynamic(this, &UDamageReceiverComponent::RadialDamage);
	// ...
	
}

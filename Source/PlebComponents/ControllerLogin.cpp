// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerLogin.h"

// Sets default values for this component's properties
UControllerLogin::UControllerLogin()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	LoggedIn = false;
}


// Called when the game starts
void UControllerLogin::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(RetryHandle, this, &UControllerLogin::TryLogin, 1.f/15.f, true);	
}

void UControllerLogin::TryLogin_Implementation()
{
	ResolveLogin();
}

void UControllerLogin::ResolveLogin_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(RetryHandle);
	LoggedIn = true;
	OnLogin.Broadcast(Cast<APlayerController>(GetOwner()));
}


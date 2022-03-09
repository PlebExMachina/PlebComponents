// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TeamComponent.generated.h"

/*
* Represents the "Team" that an Actor may be a part of.
* Information needs to be shared between actors so this should be part of the Actor proper and not the controller.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLEBCOMPONENTS_API UTeamComponent : public UActorComponent
{
	GENERATED_BODY()

	protected:
	
	// Identifies what team the Actor is on.
	UPROPERTY(EditAnywhere, Replicated)
	FName Team;

	// Identifies other team members.
	UPROPERTY(Replicated)
	TArray<UTeamComponent*> TeamMembers;

public:	
	// Sets default values for this component's properties
	UTeamComponent();

	// Getters

	UFUNCTION(BlueprintPure)
	FName GetTeam();

	// Setters (Server Only)

	UFUNCTION(BlueprintCallable)
	void SetTeam(FName New);

	UFUNCTION(BlueprintCallable)
	void SetTeamMembers(const TArray<UTeamComponent*> New);


	// Listeners, Bind these to delegates responsible for managing the team members entering / exiting.
	UFUNCTION(BlueprintCallable)
	void OnPotentialTeamMemberEnter(UTeamComponent* New);

	// Listeners, Bind these to delegates responsible for managing the team members entering / exiting.
	UFUNCTION(BlueprintCallable)
	void OnPotentialTeamMemberExit(UTeamComponent* New);
};

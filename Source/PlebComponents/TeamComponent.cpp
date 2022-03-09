// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamComponent.h"
#include "Net/UnrealNetwork.h"

namespace {
	auto IsServer = [](UObject* o) -> bool { return nullptr != o->GetWorld()->GetAuthGameMode(); };
};

// Sets default values for this component's properties
UTeamComponent::UTeamComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	Team = NAME_None;
	TeamMembers = {};
}

void UTeamComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTeamComponent, Team);
	DOREPLIFETIME(UTeamComponent, TeamMembers);
}



FName UTeamComponent::GetTeam() {
	return Team;
}

void UTeamComponent::SetTeam(FName New) {
	if (IsServer(this)) {
		Team = New;
	}
}

void UTeamComponent::SetTeamMembers(const TArray<UTeamComponent*> New) {
	if (IsServer(this)) {
		TeamMembers = New;
	}
}


void UTeamComponent::OnPotentialTeamMemberEnter(UTeamComponent* New){
	if (IsServer(this) && New) {
		if (New->Team == Team) {
			TeamMembers.AddUnique(New);
		}
	}
}

void UTeamComponent::OnPotentialTeamMemberExit(UTeamComponent* New){
	if (IsServer(this)) {
		TeamMembers.RemoveSingle(New);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericState.h"

// Sets default values for this component's properties
UGenericState::UGenericState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UGenericState::WriteInt_Implementation(FName Key, int32 Value)
{
	IntStore.Add(Key, Value);
}

void UGenericState::WriteString_Implementation(FName Key, const FString& Value)
{
	StringStore.Add(Key, Value);
}

void UGenericState::WriteName_Implementation(FName Key, FName Value)
{
	NameStore.Add(Key, Value);
}

void UGenericState::WriteBool_Implementation(FName Key, bool Value)
{
	BoolStore.Add(Key, Value);
}

int32 UGenericState::GetInt(FName Key) {
	return IntStore.FindOrAdd(Key);
}

const FString& UGenericState::GetString(FName Key) {
	return StringStore.FindOrAdd(Key);
}

FName UGenericState::GetName(FName Key) {
	return NameStore.FindOrAdd(Key);
}

bool UGenericState::GetBool(FName Key) {
	return BoolStore.FindOrAdd(Key);
}
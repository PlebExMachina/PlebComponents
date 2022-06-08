// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SnapToComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSnapToComponentDelegate, class USnapToComponent*, Comp);

UENUM(BlueprintType)
enum class ESnapMode : uint8 {
	Actor,
	Component,
	Manual,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLEBCOMPONENTS_API USnapToComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USnapToComponent();

	// Whether pitch should be taken into account when snapping.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool bRotatePitch;

	// Whether yaw should be taken into account when snapping.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool bRotateYaw;

	// Whether X position should be taken into account when snapping.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool bTranslateX;

	// Whether Y position should be taken into account when snapping.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool bTranslateY;

	// Whether Z position should be taken into account when snapping.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool bTranslateZ;

	// How quickly to snap rotation to the focus.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float fRotateSpeed;

	// How quickly to snap translation to the focus.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float fTranslateSpeed;

	// Determines what to use when snapping actors towards a target.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	ESnapMode SnapMode;

	// Event Fired when Focus begins or an Actor is added when focus has already started.
	UPROPERTY(BlueprintAssignable)
	FSnapToComponentDelegate OnFocusStarted;

	// Event Fired an Actor has completed rotating / translating towards the focus. This can potentially fire per tick so use responsibly.
	UPROPERTY(BlueprintAssignable)
	FSnapToComponentDelegate OnFocusFinished;

	// Event Fired when an actor is untracked when focus has already started or when translation / rotation has encountered an issue. 
	UPROPERTY(BlueprintAssignable)
	FSnapToComponentDelegate OnFocusInterrupted;

protected:
	UPROPERTY(Replicated)
	AActor* _SnapToActor;

	UPROPERTY(Replicated)
	USceneComponent* _SnapToComponent;

	UPROPERTY(Replicated)
	FVector _SnapToLocation;

	UPROPERTY(ReplicatedUsing = "OnRep_SetTick")
	bool _bIsTicking;

	const FVector _GetSnapToLocation(AActor* Actor);

	const FRotator _GetSnapToRotation(AActor* Actor);

	UFUNCTION()
	void OnRep_SetTick();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Sets the Snap to reference. Only works on Server.
	UFUNCTION(BlueprintCallable)
	void SetSnapToActor(AActor* FocusActor);

	// Sets the Snap to reference. Only works on Server.
	UFUNCTION(BlueprintCallable)
	void SetSnapToComponent(USceneComponent* FocusComponent);

	// Sets the Snap to reference. Only works on Server.
	UFUNCTION(BlueprintCallable)
	void SetSnapToLocation(const FVector& FocusLocation);

	// Begins focusing for actors in the array. Only works on Server.
	UFUNCTION(BlueprintCallable)
	void BeginFocus();

	// Ends focusing for actors in the array. Only works on Server.
	UFUNCTION(BlueprintCallable)
	void EndFocus();
};

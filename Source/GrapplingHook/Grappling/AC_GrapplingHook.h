// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AC_GrapplingHook.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GRAPPLINGHOOK_API UAC_GrapplingHook : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAC_GrapplingHook();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	
	// Private Members
	FVector _playerPosition;
	FVector _grapplePoint;
	float _ropeLength;
	
	UCharacterMovementComponent* _characterMovementComponent;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	// Custom Methods
	UFUNCTION(BlueprintCallable) void CastGrapplingHook(); 
	UFUNCTION(BlueprintCallable) void OnGrapple(float deltaTime, float swingSpeed);
	UFUNCTION(BlueprintCallable) void OnGrappleHookPull(float deltaTime, float pullStrength);
	UFUNCTION(BlueprintCallable) void OnGrappleHookPullRelease();
	UFUNCTION(BlueprintCallable) void StopGrapple();
	
	// Custom Fields
	UPROPERTY(EditAnywhere) float GrapplingHookCastDistance = 250;	
	UPROPERTY(BlueprintReadOnly) bool IsGrappling;
	UPROPERTY(BlueprintReadWrite) bool IsPullingTowardsGrapplePoint;
};

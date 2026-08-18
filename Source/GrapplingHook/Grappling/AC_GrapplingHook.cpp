// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_GrapplingHook.h"

#include "ComponentUtils.h"
#include "GrappleableObject.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UAC_GrapplingHook::UAC_GrapplingHook()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAC_GrapplingHook::BeginPlay()
{
	Super::BeginPlay();

	_characterMovementComponent = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
	_characterMovementComponent->SetMovementMode(MOVE_Walking);
}

// Called every frame
void UAC_GrapplingHook::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UAC_GrapplingHook::CastGrapplingHook()
{
	// Declaring and Initializing vars
	FVector startPos = this->GetOwner()->GetActorLocation();
	FVector endPos = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraRotation().Vector();
	FCollisionQueryParams TraceParams(FName(TEXT("")), true, GetOwner());
	
	// HitResults
	FHitResult hitResult;
	
	endPos = (endPos * GrapplingHookCastDistance) + startPos;
	
	if (GetWorld()->LineTraceSingleByChannel(hitResult, startPos, endPos, ECollisionChannel::ECC_Visibility, TraceParams))
	{
		if (hitResult.GetActor()->IsA(AGrappleableObject::StaticClass()))
		{
			IsGrappling = true;
			_grapplePoint = hitResult.ImpactPoint;
			_playerPosition = this->GetOwner()->GetActorLocation();
			_ropeLength = (_playerPosition - _grapplePoint).Size(); 
			DrawDebugLine(GetWorld(), startPos, endPos, FColor::Red, false, 5);
		}
	}
	
}

void UAC_GrapplingHook::OnGrapple(float deltaTime, float swingSpeed)
{
	_characterMovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);

	// Defaults
	// Grapple Point to Player Pos
	_playerPosition = GetOwner()->GetActorLocation();
	FVector ropeDirection = (_playerPosition - _grapplePoint).GetSafeNormal(); // Get the normal direction from GrapplePoint to PlayerPos
	FVector plrVelocity = GetOwner()->GetVelocity();
	FVector gravity = FVector(0, 0, UPhysicsSettings::Get()->DefaultGravityZ);
	
	//  GET CURRENT TANGENT VELOCITY
	FVector currentTangentVelocity = plrVelocity - (plrVelocity | ropeDirection) * ropeDirection; 
	
	// GET ACCELCENTRIPETAL MAGNITUDE AND VECTOR
	float accelerationCentripetalMagnitude = pow(currentTangentVelocity.Size(), 2) / _ropeLength;
	FVector accelerationCentripetalVector = -ropeDirection * accelerationCentripetalMagnitude;
	
	// GET ACCELERATION TANGENT VECTOR
	FVector accelerationTangentVector = (gravity - (gravity | ropeDirection) * ropeDirection) * swingSpeed;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, accelerationTangentVector.ToCompactString());
	
	// FINAL ACCELERATION
	FVector finalAcceleration = (accelerationCentripetalVector + accelerationTangentVector) * deltaTime;
	_characterMovementComponent->Velocity += finalAcceleration;
	
	
	// RECALCULATING
	if ((_playerPosition - _grapplePoint).Size() > _ropeLength) // Is the new ropeLength greater?
	{
		float radialVelocity = (_characterMovementComponent->Velocity | ropeDirection);
		
		//ropeDirection = (_playerPosition - _grapplePoint).GetSafeNormal();
		FVector constrainedPositionToRope = _grapplePoint + ropeDirection * _ropeLength;
		
		_characterMovementComponent->Velocity -= radialVelocity * ropeDirection;
		GetOwner()->SetActorLocation(constrainedPositionToRope);
	} 
}

void UAC_GrapplingHook::StopGrapple()
{
	_characterMovementComponent->SetMovementMode(EMovementMode::MOVE_Falling);
	IsGrappling = false;
}



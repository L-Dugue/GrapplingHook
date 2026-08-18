// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GrappleableObject.generated.h"

UCLASS()
class GRAPPLINGHOOK_API AGrappleableObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrappleableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Fields
	UPROPERTY(EditAnywhere) UBoxComponent* GrappleableArea;
};

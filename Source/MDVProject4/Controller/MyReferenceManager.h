// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AMyController.h"
#include "GameFramework/Actor.h"
#include "MDVProject4/Player/MyPawn.h"
#include "MyReferenceManager.generated.h"

class AMyController;
class AMyActor;

UCLASS()
class MDVPROJECT4_API AMyReferenceManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyReferenceManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY()
	AMyPawn* MyPawn;

	UPROPERTY()
	AMyActor* MyActor;

	UPROPERTY()
	AMyController* MyController;

	UPROPERTY()
	AMyHUD* MyHUD;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

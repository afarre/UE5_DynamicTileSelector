// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/CheckBox.h"
#include "Components/ComboBox.h"
#include "GameFramework/Actor.h"
#include "MDVProject4/Controller/MyReferenceManager.h"
#include "AMyActor.generated.h"


UCLASS()
class MDVPROJECT4_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Create the collision capsule
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* MaterialInterface;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION(BlueprintCallable)
	void WallSelected();

	UFUNCTION(BlueprintCallable)
	void WallHovered() const;

	UFUNCTION(BlueprintCallable)
	void WallUnHovered() const;
	
	UPROPERTY()
	AMyReferenceManager* MyReferenceManager;

	/*UPROPERTY()
	UMaterialInstance* GlowOverlayMat;*/

	UPROPERTY()
	UMaterialInstance* HorizontalStrippedOverlayMat;

	UPROPERTY()
	UMaterialInstance* VerticalStrippedOverlayMat;

	UPROPERTY(EditAnywhere)
	bool IsHorizontalWall;
	
	/*
	UPROPERTY(EditAnywhere, CallInEditor)
	UComboBox* Test;*/
};

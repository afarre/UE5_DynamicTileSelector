// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class AMyReferenceManager;
class AMyActor;

UCLASS()
class MDVPROJECT4_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* PawnMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ClickAction;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	AMyActor* SelectedWall;

	UFUNCTION(BlueprintCallable)
	void OnClick();

	UPROPERTY()
	AMyReferenceManager* MyReferenceManager;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

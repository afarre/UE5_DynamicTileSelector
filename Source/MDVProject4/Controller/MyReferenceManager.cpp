// Fill out your copyright notice in the Description page of Project Settings.


#include "MyReferenceManager.h"

#include "Kismet/GameplayStatics.h"
#include "MDVProject4/Objects/AMyActor.h"
#include "MDVProject4/UI/HUD/MyHUD.h"


// Sets default values
AMyReferenceManager::AMyReferenceManager() {
 	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AMyReferenceManager::BeginPlay() {
	Super::BeginPlay();
	
	MyPawn = Cast<AMyPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyPawn::StaticClass()));

	MyActor = Cast<AMyActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyActor::StaticClass()));

	MyController = Cast<AMyController>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyController::StaticClass()));
	
	MyHUD = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

// Called every frame
void AMyReferenceManager::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}


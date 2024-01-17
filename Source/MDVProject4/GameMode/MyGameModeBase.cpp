// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"

#include "MDVProject4/UI/HUD/MyHUD.h"
#include "MDVProject4/Player/MyPawn.h"


void AMyGameModeBase::BeginPlay() {
	Super::BeginPlay();
}

AMyGameModeBase::AMyGameModeBase() {
	HUDClass = AMyHUD::StaticClass();
	DefaultPawnClass = AMyPawn::StaticClass();
}

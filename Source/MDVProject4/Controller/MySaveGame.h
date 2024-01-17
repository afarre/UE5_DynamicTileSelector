// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Containers/Map.h"
#include "MDVProject4/Objects/AMyActor.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class MDVPROJECT4_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<AActor*> MyWalls;

	UPROPERTY()
	TArray<FString> TextureSourceFileName;

	UPROPERTY()
	TMap<AMyActor*, FString> SaveMap;

	UPROPERTY()
	FString ResourcesDir;

	UPROPERTY()
	FString ScreenshotDir;
};

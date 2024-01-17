// Fill out your copyright notice in the Description page of Project Settings.


#include "AMyActor.h"

#include "Kismet/GameplayStatics.h"
#include "MDVProject4/Controller/AMyController.h"
#include "MDVProject4/Utils/Defines.h"

// Sets default values
AMyActor::AMyActor() {
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Brush"));
	StaticMesh->SetupAttachment(RootComponent);

	//GlowOverlayMat = LoadObject<UMaterialInstance>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/MI_Glow.MI_Glow'"));

	HorizontalStrippedOverlayMat = LoadObject<UMaterialInstance>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/MI_StrippedHorizontal.MI_StrippedHorizontal'"));
	VerticalStrippedOverlayMat = LoadObject<UMaterialInstance>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/MI_StrippedVertical.MI_StrippedVertical'"));
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay() {
	Super::BeginPlay();
	StaticMesh->SetMaterial(M_MAT_NUM, MaterialInterface);

	MyReferenceManager = Cast<AMyReferenceManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyReferenceManager::StaticClass()));
}


// Called every frame
void AMyActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AMyActor::WallSelected() {
	if (MyReferenceManager->MyController->IsTileSelectEnabled()) {
		MyReferenceManager->MyController->UpdateSelectedWall(this);
	}
}

void AMyActor::WallHovered() const {
	if (MyReferenceManager->MyController->IsTileSelectEnabled()) {
		if (IsHorizontalWall) {
			StaticMesh->SetOverlayMaterial(HorizontalStrippedOverlayMat);
		} else {
			StaticMesh->SetOverlayMaterial(VerticalStrippedOverlayMat);
		}
		MyReferenceManager->MyController->WallHovered = true;
		//StaticMesh->SetOverlayMaterial(GlowOverlayMat);
	}
}

void AMyActor::WallUnHovered() const {
	if (MyReferenceManager->MyController->IsTileSelectEnabled()) {
		MyReferenceManager->MyController->WallHovered = false;
		StaticMesh->SetOverlayMaterial(nullptr);
	}
}


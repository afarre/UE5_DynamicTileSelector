// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"

#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "MDVProject4/Controller/MyReferenceManager.h"

// Sets default values
AMyPawn::AMyPawn() {
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay() {
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	MyReferenceManager = Cast<AMyReferenceManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyReferenceManager::StaticClass()));
	
    if (PlayerController) {
        APlayerController* MyController = GetWorld()->GetFirstPlayerController();
        // Enable cursor
        MyController->bShowMouseCursor = true; 
        MyController->bEnableClickEvents = true; 
        MyController->bEnableMouseOverEvents = true;

    	// Get the "Enhanced Input Local Player Subsystem" blueprint
    	
    	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
    		// Assign our mapping context to the aforementioned Subsystem
    		Subsystem->AddMappingContext(PawnMappingContext, 0);
    	}
    }
}

void AMyPawn::OnClick() {
	MyReferenceManager->MyController->ScreenClicked();
	
	UWidgetLayoutLibrary::GetMousePositionOnPlatform();
}

// Called every frame
void AMyPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &AMyPawn::OnClick);
	}
}

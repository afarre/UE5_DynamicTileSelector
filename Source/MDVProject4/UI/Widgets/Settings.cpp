// Fill out your copyright notice in the Description page of Project Settings.


#include "MDVProject4/UI/Widgets/Settings.h"

#include "MDVProject4/UI/HUD/MyHUD.h"


void USettings::NativeOnInitialized() {
	Super::NativeOnInitialized();
	MyHUD = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void USettings::ChangeScreenshotDirPressed() {
	MyHUD->ChangeDir(false);
}

void USettings::ChangeResourcesDirPressed() {
	MyHUD->ChangeDir(true);
}

void USettings::OpenScreenshotDirPressed() {
	MyHUD->OpenDir(false);
}

void USettings::OpenResourcesDirPressed() {
	MyHUD->OpenDir(true);
}

void USettings::CloseButtonPressed() {
	RemoveFromParent();
	MyHUD->EnableTileSelect();
}

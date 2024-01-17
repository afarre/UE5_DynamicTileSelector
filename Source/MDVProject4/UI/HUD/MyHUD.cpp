// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "IDesktopPlatform.h"
#include "DesktopPlatformModule.h"
#include "Engine/Texture2D.h"
#include "MDVProject4/Controller/MyReferenceManager.h"
#include "MDVProject4/UI/Widgets/AlertDialog.h"

void AMyHUD::BeginPlay() {
	MyReferenceManager = Cast<AMyReferenceManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyReferenceManager::StaticClass()));
	if (TileSelectWidget) {
		// Create an object of class TileSelect if a blueprint has been set in the editor
		TileSelect = CreateWidget<UTileSelect>(GetWorld()->GetFirstPlayerController(), TileSelectWidget);
		TileSelect->PopulateWidgets(MyReferenceManager->MyController->DynamicMaterialArray);
		
		// If the instance was created correctly, display on screen
		if (TileSelect) {
			TileSelect->AddToViewport();
		}
	}

	if (AlertDialogWidget) {
		AlertDialog = CreateWidget<UAlertDialog>(GetWorld()->GetFirstPlayerController(), AlertDialogWidget);
	}

	if (NotifierWidget) {
		Notifier = CreateWidget<UNotifier>(GetWorld()->GetFirstPlayerController(), NotifierWidget);
	}

	if (ScreenshotEffectWidget) {
		ScreenshotEffect = CreateWidget<UScreenshotEffect>(GetWorld()->GetFirstPlayerController(), ScreenshotEffectWidget);
	}

	if (SettingsWidget) {
		Settings = CreateWidget<USettings>(GetWorld()->GetFirstPlayerController(), SettingsWidget);
	}
}

/**
 * Notifies the controller that a user interaction has been performed to change a wall's material
 * @param DynamicMaterial The material that needs to be set on the wall, nullptr when the default material is desired
 */
void AMyHUD::UpdateWallMaterial(UMaterialInstanceDynamic* DynamicMaterial) const {
	if (DynamicMaterial) {
		MyReferenceManager->MyController->SetWallMaterial(DynamicMaterial);	
	}else {
		MyReferenceManager->MyController->SetDefaultMaterial();
	}
}

/**
 * Notifies the TileSelect widget that the SelectedWallTextBox needs to be updated with the wall's name
 * @param SelectedWall Wall that was selected (it contains the Wall name in a tag)
 */
void AMyHUD::UpdateSelectedWallText(AMyActor* SelectedWall) const {
	if (SelectedWall) {
		TileSelect->UpdateText(SelectedWall->Tags[1].ToString());
	}else {
		TileSelect->UpdateText("-");
	}
}

/**
 * Notifies the TileSelected widget that he must refresh the dynamic buttons
 */
void AMyHUD::RefreshTilesWidget(const TArray<FMyDynamicMat>& DynamicMaterialArray) const {
	TileSelect->RefreshWidget(DynamicMaterialArray);
}

/**
 * Notifies the controller that the Save button has been pressed
 */
void AMyHUD::SaveGameButtonPressed() const {
	MyReferenceManager->MyController->SaveGame();
}

/**
 * Notifies the controller that the Load button has been pressed
 */
void AMyHUD::LoadGameButtonPressed() const {
	MyReferenceManager->MyController->LoadGame();
}

/**
 * Notifies the controller that the Delete button has been pressed
 */
void AMyHUD::DeleteButtonPressed() const {
	MyReferenceManager->MyController->DeleteSaveFile();
}

/**
 * Notifies the AlertDialog that it must be displayed with the screenshot widgets
 */
void AMyHUD::DisplayScreenshotDialog() const {
	AlertDialog->PopulateWidget();
	AlertDialog->AddToViewport();
	DisableTileSelect();
}

/**
 * Notifies the AlertDialog that it must be displayed with the missing files widgets
 * @param MissingFiles TMap <Material, Wall> containing the missing materials and their respective walls
 */
void AMyHUD::DisplayMissingFilesDialog(const TMap<FString, FString>& MissingFiles) const {
	if (AlertDialog) {
		AlertDialog->PopulateWidget(MissingFiles);
		AlertDialog->AddToViewport();
	}
}

/**
 * Notifies the Notify that is must be displayed with the shared type and message
 * @param NotifyType ENotifyType indicating the type of notification to display
 * @param Message The content of the notification
 */
void AMyHUD::Notify(const ENotifyType NotifyType, const FText& Message) const {
	UTexture2D* Icon = nullptr;
	switch (NotifyType){
	case Info:
		Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Resources/Icons/info.info'"));
		break;
		
	case Warning:
		Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Resources/Icons/Warning.Warning'"));
		break;
		
	case Error:
		Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Resources/Icons/Error.Error'"));
		break;
	}
	
	if (Icon) {
		Notifier->SetIcon(Icon);
	}

	Notifier->SetMessageText(Message);
	if (Notifier->IsInViewport()) {
		Notifier->RemoveFromParent();	
	}
	Notifier->AddToViewport();
	Notifier->PlayAnimation(Notifier->Anim, 0, 1);
}

/**
 * Notifies the ScreenshotEffect that it must display the screenshot image and it's corresponding animation
 * @param ScreenshotTexture The image to display in the screenshot effect
 */
void AMyHUD::TriggerScreenshotEffect(UTexture2D* ScreenshotTexture) const {
	if (ScreenshotTexture->IsValidLowLevel() && ScreenshotEffect) {
		ScreenshotEffect->SetScreenshotTexture(ScreenshotTexture);
		ScreenshotEffect->AddToViewport();
		ScreenshotEffect->PlayAnimation(ScreenshotEffect->Anim,0 , 1);
	}
}

/**
 * Notifies the controller that it must create a screenshot, and re-enables input on the main (TileSelect) UI widget
 * @param ScreenshotName The name given to the screenshot by the user
 */
void AMyHUD::ScreenshotNameSelected(const FText& ScreenshotName) const {
	MyReferenceManager->MyController->CreateScreenshot(ScreenshotName);
	AlertDialog->RemoveFromParent();
	EnableTileSelect();
}

FText AMyHUD::RetrieveDataTableMessage(const EDataTableContentIndex DataTableContentIndex) const {
	return MyReferenceManager->MyController->RetrieveDataTableMessage(DataTableContentIndex);
}

/**
 * Notifies the Settings widget that it must be displayed and disables input on the main (TileSelect) UI widget
 */
void AMyHUD::SettingsPressed() const {
	Settings->AddToViewport();
	DisableTileSelect();
}

void AMyHUD::ChangeDir(bool IsResourcesDir) {
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform) {
		FString FolderDir;
		const void* ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
		// TODO: Remove magic string
		const FString Title = "Select a directory";

		//TODO: Beautify
		if (IsResourcesDir) {
			DesktopPlatform->OpenDirectoryDialog(ParentWindowHandle, Title, MyReferenceManager->MyController->ResourcesDir, FolderDir);
			if (!FolderDir.IsEmpty()) {
				MyReferenceManager->MyController->UpdateResourcesDir(FolderDir);
				Notify(Info, RetrieveDataTableMessage(ResourcesDirSetOK));	
			}
		} else {
			DesktopPlatform->OpenDirectoryDialog(ParentWindowHandle, Title, MyReferenceManager->MyController->ScreenshotDir, FolderDir);
			if (!FolderDir.IsEmpty()) {
				MyReferenceManager->MyController->UpdateScreenshotDir(FolderDir);
				Notify(Info, RetrieveDataTableMessage(ResourcesDirSetOK));	
			}
		}
	}
}

void AMyHUD::OpenDir(bool IsResourcesDir) {
	FString Dir;
	if (IsResourcesDir) {
		Dir = MyReferenceManager->MyController->ResourcesDir;
		//UE_LOG(LogTemp, Warning, TEXT("Set resources dir"))
	} else {
		Dir = MyReferenceManager->MyController->ScreenshotDir;
		//UE_LOG(LogTemp, Warning, TEXT("Set screenshot dir"))
	}

	if (Dir.IsEmpty()) {
		//UE_LOG(LogTemp, Warning, TEXT("dir is empty, display message"))
		Notify(Error, RetrieveDataTableMessage(IncorrectPath));
		return;
	}

	const FString FullPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*Dir);
	if (FPaths::ValidatePath(FullPath) && FPaths::DirectoryExists(FullPath)) {
		FPlatformProcess::ExploreFolder(*FullPath);
	} else {
		Notify(Error, RetrieveDataTableMessage(IncorrectPath));	
	}
}

/**
 * Enables input on the main (TileSelect) UI widget
 */
void AMyHUD::EnableTileSelect() const {
	TileSelect->Enable();
}

/**
 * Disables input on the main (TileSelect) UI widget
 */
void AMyHUD::DisableTileSelect() const {
	TileSelect->Disable();
}

/**
 * Checks if the input is enabled for the main (TileSelect) UI widget
 * @return True if the input on the main (TileSelect) UI widget is enabled
 */
bool AMyHUD::IsTileSelectEnabled() const {
	return TileSelect->GetVisibility() == ESlateVisibility::SelfHitTestInvisible;
}

void AMyHUD::DebugPressed() {
	MyReferenceManager->MyController->DebugPressed();
}




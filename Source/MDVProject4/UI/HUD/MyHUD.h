// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MDVProject4/Objects/AMyActor.h"
#include "MDVProject4/UI/Widgets/AlertDialog.h"
#include "MDVProject4/UI/Widgets/Notifier.h"
#include "MDVProject4/UI/Widgets/ScreenshotEffect.h"
#include "MDVProject4/UI/Widgets/Settings.h"
#include "MDVProject4/UI/Widgets/TileSelect.h"

#include "MyHUD.generated.h"

class AMyReferenceManager;
/**
 * 
 */
UCLASS()
class MDVPROJECT4_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	void UpdateWallMaterial(UMaterialInstanceDynamic* DynamicMaterial) const;
	
	void UpdateSelectedWallText(AMyActor* SelectedWall) const;
	
	void RefreshTilesWidget(const TArray<FMyDynamicMat>& DynamicMaterialArray) const;
	
	void SaveGameButtonPressed() const;
	void LoadGameButtonPressed() const;
	void DeleteButtonPressed() const;
	
	void DisplayScreenshotDialog() const;
	
	void DisplayMissingFilesDialog(const TMap<FString, FString>& MissingFiles) const;
	
	void Notify(ENotifyType NotifyType, const FText& Message) const;
	
	void TriggerScreenshotEffect(UTexture2D* ScreenshotTexture) const;
	
	void ScreenshotNameSelected(const FText& ScreenshotName) const;
	
	FText RetrieveDataTableMessage(EDataTableContentIndex DataTableContentIndex) const;

	void SettingsPressed() const;
	
	void ChangeDir(bool IsResourcesDir);

	void OpenDir(bool IsResourcesDir);
	
	void EnableTileSelect() const;
	
	bool IsTileSelectEnabled() const;

	void DebugPressed();

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> TileSelectWidget;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> AlertDialogWidget;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> NotifierWidget;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> TreeViewEntryWidget;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> ScreenshotEffectWidget;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> SettingsWidget;
	
private:
	UPROPERTY()
	AMyReferenceManager* MyReferenceManager;

	UPROPERTY()
	UTileSelect* TileSelect;
	
	UPROPERTY()
	UAlertDialog* AlertDialog;
	
	UPROPERTY()
	UNotifier* Notifier;

	UPROPERTY()
	UScreenshotEffect* ScreenshotEffect;

	UPROPERTY()
	USettings* Settings;
	
	UPROPERTY()
	UTexture* InfoIcon;
	UPROPERTY()
	UTexture2D* WarningIcon;
	UPROPERTY()
	UTexture2D* ErrorIcon;

	void DisableTileSelect() const;
};

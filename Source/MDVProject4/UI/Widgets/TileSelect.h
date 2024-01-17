// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyButton.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "MDVProject4/Utils/DataStructures.h"

#include "TileSelect.generated.h"


class AMyHUD;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class MDVPROJECT4_API UTileSelect : public UUserWidget {
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	void PopulateWidgets(TArray<FMyDynamicMat> MyDynamicArray);
	
	void RefreshWidget(const TArray<FMyDynamicMat> MyDynamicArray);

	void UpdateText(const FString& WallName);

	void Disable();
	void Enable();

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* SelectedWallTextBox;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UOverlay* BaseOverlay;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UUniformGridPanel* UniformGridPanel;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* Settings;

private:
	UFUNCTION(BlueprintCallable)
	void DefaultPressed() const;

	UFUNCTION(BlueprintCallable)
	void SavePressed() const;

	UFUNCTION(BlueprintCallable)
	void LoadPressed() const;

	UFUNCTION(BlueprintCallable)
	void DeletePressed() const;

	UFUNCTION(BlueprintCallable)
	void ScreenshotPressed() const;

	UFUNCTION(BlueprintCallable)
	void SettingsPressed() const;

	UFUNCTION()
	void OnMyButtonClicked(UMyButton* Button);

	UFUNCTION(BlueprintCallable)
	void DebugPressed() const;
	
	void PopulateWidgetWithDynamicMaterialArray();

	void GetBaseWidgets();

	UPROPERTY()
	AMyHUD* MyHUD;

	UPROPERTY()
	TArray<FMyDynamicMat> DynamicMaterialArray;

	UPROPERTY()
	UImage* BaseImage;
	UPROPERTY()
	UOverlay* BaseInternalOverlay;
	UPROPERTY()
	UMyButton* BaseButton;
};

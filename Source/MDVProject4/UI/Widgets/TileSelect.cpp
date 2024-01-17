// Fill out your copyright notice in the Description page of Project Settings.


#include "TileSelect.h"

#include "MyButton.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/PanelWidget.h"
#include "MDVProject4/Controller/AMyController.h"
#include "MDVProject4/UI/HUD/MyHUD.h"

void UTileSelect::NativeOnInitialized() {
	Super::NativeOnInitialized();
	MyHUD = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

/**
 * Triggered when the widget's "Default" button is pressed
 */
void UTileSelect::DefaultPressed() const {
	MyHUD->UpdateWallMaterial(nullptr);
}

/**
 * Triggered when the widget's "Save" button is pressed
 */
void UTileSelect::SavePressed() const {
	MyHUD->SaveGameButtonPressed();
}

/**
 * Triggered when the widget's "Load" button is pressed
 */
void UTileSelect::LoadPressed() const {
	MyHUD->LoadGameButtonPressed();
}

/**
 * Triggered when the widget's "Delete" button is pressed
 */
void UTileSelect::DeletePressed() const {
	MyHUD->DeleteButtonPressed();
}

/**
 * Triggered when the widget's "Take Screenshot" button is pressed
 */
void UTileSelect::ScreenshotPressed() const {
	MyHUD->DisplayScreenshotDialog();
}

/**
 * Sets the DynamicMaterialArray and makes calls to display it on screen
 * @param MyDynamicArray  Array containing the tile's information we must display
 */
void UTileSelect::PopulateWidgets(TArray<FMyDynamicMat> MyDynamicArray) {
	DynamicMaterialArray = MyDynamicArray;
	GetBaseWidgets();
	PopulateWidgetWithDynamicMaterialArray();
}

/**
 * Gets all base widget references from the existing placeholder widget
 */
void UTileSelect::GetBaseWidgets() {
	// Get pointers to existing components in the BaseOverlay widget
	BaseImage = Cast<UImage>(BaseOverlay->GetChildAt(0));
	BaseInternalOverlay = Cast<UOverlay>(BaseOverlay->GetChildAt(1));
	BaseButton = Cast<UMyButton>(BaseInternalOverlay->GetChildAt(0));
}

/**
 * Iterates the DynamicMaterialArray extracting & parsing each tile's information to add them to the UniformGridPanel
 */
void UTileSelect::PopulateWidgetWithDynamicMaterialArray() {
	UniformGridPanel->ClearChildren();
	int Row = 0, Column = 0;
	for (FMyDynamicMat Element : DynamicMaterialArray) {
    	// Duplicate the existing items
    	UOverlay* NewOverlay = DuplicateObject<UOverlay>(BaseOverlay, BaseOverlay->GetOuter(), FName(Element.CleanName));
    	UImage* Image = DuplicateObject<UImage>(BaseImage, BaseOverlay->GetOuter());
    	UOverlay* InternalOverlay = DuplicateObject<UOverlay>(BaseInternalOverlay, BaseOverlay->GetOuter());
    	UMyButton* Button = DuplicateObject<UMyButton>(BaseButton, InternalOverlay->GetOuter());

    	Button->OnClickedDelegate.AddUniqueDynamic(this, &ThisClass::OnMyButtonClicked);
    	
    	// Modify the duplicated items
    	Image->SetBrushFromTexture(Element.Texture2D, false);
    	
    	// Replace existing base items with the modified duplicated items
    	//NewOverlay->ReplaceChild(BaseImage, Image);
		NewOverlay->ReplaceOverlayChildAt(0, Image);
		NewOverlay->ReplaceOverlayChildAt(1, InternalOverlay);
    	//NewOverlay->ReplaceChild(BaseInternalOverlay, InternalOverlay);
    	//NewOverlay->ReplaceChild(BaseButton, Button);
    	InternalOverlay->ClearChildren();
    	InternalOverlay->AddChild(Button);
    	
    	UniformGridPanel->AddChildToUniformGrid(NewOverlay, Row, Column);
    	if (Column == 1) {
    		Row++;
    		Column = 0;
    	} else {
    		Column++;
    	}
    }
}

/**
 * Called when a dynamically created button depicting a tile has been clicked on
 * @param Button Which button has been clicked
 */
void UTileSelect::OnMyButtonClicked(UMyButton* Button) {
	for (const FMyDynamicMat Element : DynamicMaterialArray) {
		if (Element.CleanName == FName(Button->GetParent()->GetParent()->GetName())) {
			MyHUD->UpdateWallMaterial(Element.DynamicMaterial);
			break;
		}
	}
}

void UTileSelect::DebugPressed() const {
	MyHUD->DebugPressed();
}

void UTileSelect::UpdateText(const FString& WallName) {
	if (SelectedWallTextBox) {
		SelectedWallTextBox->SetText(FText::FromString(WallName));
	}
}

/**
 * Refreshed the DynamicMaterialArray data and makes a call to redraw the widget's UniformGridPanel
 * @param MyDynamicArray Array containing the new set of data that needs to be displayed
 */
void UTileSelect::RefreshWidget(const TArray<FMyDynamicMat> MyDynamicArray) {
	DynamicMaterialArray = MyDynamicArray;
	PopulateWidgetWithDynamicMaterialArray();
}

void UTileSelect::SettingsPressed() const {
	MyHUD->SettingsPressed();
}

void UTileSelect::Disable() {
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UTileSelect::Enable() {
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

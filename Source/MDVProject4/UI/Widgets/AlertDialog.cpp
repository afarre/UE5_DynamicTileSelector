// Fill out your copyright notice in the Description page of Project Settings.


#include "MDVProject4/UI/Widgets/AlertDialog.h"
#include "MDVProject4/UI/HUD/MyHUD.h"

void UAlertDialog::NativeOnInitialized() {
	Super::NativeOnInitialized();
	MyHUD = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

/**
 * Triggered when the Close button is pressed on the widget
 */
void UAlertDialog::CloseButtonPressed() {
	RemoveFromParent();
	EditableText->SetText(FText::FromString(""));
	MyHUD->EnableTileSelect();
}

/**
 * Triggered when the Accept button is pressed on the widget
 */
void UAlertDialog::AcceptButtonPressed() {
	FDataTableContent DataTableContent;
	IncorrectNameText->SetVisibility(ESlateVisibility::Collapsed);
	Title->SetVisibility(ESlateVisibility::Collapsed);
	if (EditableText->GetText().IsEmpty()) {
		IncorrectNameText->SetText(MyHUD->RetrieveDataTableMessage(IncorrectScreenshotName));
		IncorrectNameText->SetVisibility(ESlateVisibility::Visible);
	}else {
		MyHUD->ScreenshotNameSelected(EditableText->GetText());
		EditableText->SetText(FText::FromString(""));
		// TODO EditableText->OnTextCommitted
	}
}

void UAlertDialog::PopulateWidget(TMap<FString, FString> MissingFiles) {
	Title->SetVisibility(ESlateVisibility::Visible);
	MessageBody->SetText(MyHUD->RetrieveDataTableMessage(MissingTexturesMessage));
	EditableText->SetVisibility(ESlateVisibility::Collapsed);
	TreeView->SetVisibility(ESlateVisibility::Visible);
	IncorrectNameText->SetVisibility(ESlateVisibility::Collapsed);
	AcceptButton->SetVisibility(ESlateVisibility::Collapsed);
	
	TArray<UTreeViewEntry*> TreeViewEntries;
	
	for (TPair<FString, FString>& Element : MissingFiles) {
		bool Contains = false;
		// Check if the TreeViewEntries already contains the texture source file. If so, add the wall as a children
		if (!TreeViewEntries.IsEmpty()) {
			for (UTreeViewEntry* TreeViewEntryElement : TreeViewEntries) {
				if (Element.Value == TreeViewEntryElement->HeaderText->GetText().ToString()) {
					Contains = true;
					UTextBlock* NewBodyText = DuplicateObject<UTextBlock>(TreeViewEntryElement->BodyText, TreeViewEntryElement->BodyVerticalBox->GetOuter());
					NewBodyText->SetText(FText::FromString(Element.Key));
					TreeViewEntryElement->BodyVerticalBox->AddChild(NewBodyText);
					break;
				}
			}
		}
		
		if (!Contains) {
			if (MyHUD->TreeViewEntryWidget) {
				UTreeViewEntry* NewTreeViewEntry = CreateWidget<UTreeViewEntry>(GetWorld(), MyHUD->TreeViewEntryWidget);
				NewTreeViewEntry->HeaderText->SetText(FText::FromString(Element.Value));
				NewTreeViewEntry->BodyText->SetText(FText::FromString(Element.Key));
				TreeViewEntries.Add(NewTreeViewEntry);
			}
		}
	}

	//TODO: Look for a better way to populate the tree view

	/*for (auto asd : TreeViewEntries) {
		UE_LOG(LogTemp, Warning, TEXT("[pre] Header: %s"), *asd->HeaderText->GetText().ToString());
	}

	for (auto asd : TreeViewEntries) {
		TreeView->AddItem(asd);
	}*/
	TreeView->SetListItems(TreeViewEntries);
	//TreeView->SetSelectedItem(TreeViewEntries[0]);
	
	/*for (auto asd : TreeViewEntries) {
		UE_LOG(LogTemp, Warning, TEXT("[post] Header: %s"), *asd->HeaderText->GetText().ToString());
	}*/
}

void UAlertDialog::PopulateWidget() {
	Title->SetVisibility(ESlateVisibility::Collapsed);
	MessageBody->SetText(MyHUD->RetrieveDataTableMessage(ScreenshotNameMessage));
	EditableText->SetVisibility(ESlateVisibility::Visible);
	TreeView->SetVisibility(ESlateVisibility::Collapsed);
	IncorrectNameText->SetVisibility(ESlateVisibility::Collapsed);
	AcceptButton->SetVisibility(ESlateVisibility::Visible);
}

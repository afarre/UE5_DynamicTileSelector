// Fill out your copyright notice in the Description page of Project Settings.

#include "MDVProject4/UI/Widgets/TreeViewEntry.h"


void UTreeViewEntry::NativeOnListItemObjectSet(UObject* ListItemObject) {
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	
	UTreeViewEntry* TreeViewEntry = Cast<UTreeViewEntry>(ListItemObject);
	//UE_LOG(LogTemp, Warning, TEXT("Working on %s"), *TreeViewEntry->HeaderText->GetText().ToString());
	HeaderText->SetText(TreeViewEntry->HeaderText->GetText());
	//HeaderText = TreeViewEntry->HeaderText;
	
	BodyVerticalBox->ClearChildren();
	for (UWidget* Child : TreeViewEntry->BodyVerticalBox->GetAllChildren()) {
		UTextBlock* ChildBodyText = Cast<UTextBlock>(Child);
		UTextBlock* NewBodyText = DuplicateObject<UTextBlock>(ChildBodyText, TreeViewEntry->BodyVerticalBox->GetOuter());
		NewBodyText->SetText(ChildBodyText->GetText());
		BodyVerticalBox->AddChild(NewBodyText);
	}
	//BodyVerticalBox = TreeViewEntry->BodyVerticalBox;
}

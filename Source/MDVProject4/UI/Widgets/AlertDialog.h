// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TreeViewEntry.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Components/TreeView.h"
#include "MDVProject4/Objects/AMyActor.h"
#include "AlertDialog.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class MDVPROJECT4_API UAlertDialog : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintCallable)
	void CloseButtonPressed();

	UFUNCTION(BlueprintCallable)
	void AcceptButtonPressed();

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* Title;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* CloseButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* MessageBody;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTreeView* TreeView;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UEditableText* EditableText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* AcceptButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* IncorrectNameText;

	UPROPERTY()
	AMyHUD* MyHUD;
	
	void PopulateWidget(TMap<FString, FString> MissingFiles);

	void PopulateWidget();

private:
	UPROPERTY()
	UTreeViewEntry* TreeViewEntry;
};

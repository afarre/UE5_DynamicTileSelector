// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Components/ExpandableArea.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

#include "TreeViewEntry.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class MDVPROJECT4_API UTreeViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UExpandableArea* ExpandableArea;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* HeaderText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UVerticalBox* BodyVerticalBox;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* BodyText;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};

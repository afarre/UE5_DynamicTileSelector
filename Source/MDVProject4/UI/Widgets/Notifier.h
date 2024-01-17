// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Notifier.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class MDVPROJECT4_API UNotifier : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* Message;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* Icon;

	UPROPERTY(Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* Anim;

	FWidgetAnimationDynamicEvent Delegate;
	
	virtual void NativeConstruct() override;
		
	UFUNCTION()
	void NotifyDelegateFunc();

	UFUNCTION(BlueprintCallable)
	void NotifyClicked();
	
	void SetIcon(UTexture2D* Icon);
	
	void SetMessageText(const FText& MessageText);
};

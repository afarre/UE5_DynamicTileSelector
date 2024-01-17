// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "ScreenshotEffect.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class MDVPROJECT4_API UScreenshotEffect : public UUserWidget {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* ScreenshotImage;
		
	UPROPERTY(Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* Anim;

	FWidgetAnimationDynamicEvent Delegate;
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void NotifyDelegateFunc();
	
	void SetScreenshotTexture(UTexture2D* ScreenshotTexture);
};

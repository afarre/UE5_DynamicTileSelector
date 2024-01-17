// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Settings.generated.h"

class AMyHUD;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class MDVPROJECT4_API USettings : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* ChangeScreenshotDir;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* ChangeResourcesDir;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* OpenScreenshotDir;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* OpenResourcesDir;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* Close;

	/*UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* Save;*/

private:
	UPROPERTY()
	AMyHUD* MyHUD;
	
	UFUNCTION(BlueprintCallable)
	void ChangeScreenshotDirPressed();

	UFUNCTION(BlueprintCallable)
	void ChangeResourcesDirPressed();

	UFUNCTION(BlueprintCallable)
	void OpenScreenshotDirPressed();

	UFUNCTION(BlueprintCallable)
	void OpenResourcesDirPressed();

	UFUNCTION(BlueprintCallable)
	void CloseButtonPressed();
};

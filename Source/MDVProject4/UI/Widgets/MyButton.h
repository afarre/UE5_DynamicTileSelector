// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMyButtonClicked, class UMyButton*, Button);

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class MDVPROJECT4_API UMyButton : public UUserWidget {
	GENERATED_BODY()

public:
	// Bind function
	FOnMyButtonClicked OnClickedDelegate;
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* Button;
	
	UFUNCTION()
	void OnButtonClicked();
};

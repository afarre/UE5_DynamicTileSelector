// Fill out your copyright notice in the Description page of Project Settings.


#include "MyButton.h"

#include "Components/Button.h"


void UMyButton::NativeConstruct() {
	Super::NativeConstruct();

	Button->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButtonClicked);
	Button->bIsVariable = true;
}

void UMyButton::OnButtonClicked() {
	OnClickedDelegate.Broadcast(this);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "MDVProject4/UI/Widgets/Notifier.h"


void UNotifier::NativeConstruct() {
	Super::NativeConstruct();
	Delegate.BindDynamic(this, &UNotifier::NotifyDelegateFunc);
	BindToAnimationFinished(Anim, Delegate);

}

void UNotifier::NotifyDelegateFunc() {
	RemoveFromParent();
}

void UNotifier::NotifyClicked() {
	RemoveFromParent();
}

void UNotifier::SetIcon(UTexture2D* IconTexture) {
	Icon->SetBrushFromTexture(IconTexture);
}

void UNotifier::SetMessageText(const FText& MessageText) {
	Message->SetText(MessageText);
}

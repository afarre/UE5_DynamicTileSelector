// Fill out your copyright notice in the Description page of Project Settings.


#include "MDVProject4/UI/Widgets/ScreenshotEffect.h"

void UScreenshotEffect::NativeConstruct() {
	Super::NativeConstruct();
	Delegate.BindDynamic(this, &UScreenshotEffect::NotifyDelegateFunc);
	BindToAnimationFinished(Anim, Delegate);

}

void UScreenshotEffect::NotifyDelegateFunc() {
	RemoveFromParent();
}

void UScreenshotEffect::SetScreenshotTexture(UTexture2D* ScreenshotTexture) {
	ScreenshotImage->SetBrushFromTexture(ScreenshotTexture);
}

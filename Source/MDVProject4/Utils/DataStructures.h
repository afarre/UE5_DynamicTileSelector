#pragma once

#include "Engine/DataTable.h"
#include "DataStructures.generated.h"


USTRUCT()
struct FMyDynamicMat {
	GENERATED_BODY()

	UPROPERTY()
	FName CleanName;

	UPROPERTY()
	FString Path;

	UPROPERTY()
	UTexture2D* Texture2D;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	FMyDynamicMat() {
		CleanName = "NoName";
		Path = "NoPath";
		Texture2D = nullptr;
		DynamicMaterial = nullptr;
	}

	bool operator==(const FMyDynamicMat MyDynamicMat) const {
		return CleanName.IsEqual(MyDynamicMat.CleanName)
		&& Path.Equals(MyDynamicMat.Path)
		&& Texture2D == MyDynamicMat.Texture2D
		&& DynamicMaterial == MyDynamicMat.DynamicMaterial;
	}
};


UENUM()
enum ENotifyType {
	Info = 0,
	Warning = 1,
	Error = 2
};


USTRUCT(BlueprintType)
struct FDataTableContent : public FTableRowBase {
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Message;
	
	FDataTableContent() {
		Message = FText::FromString("NoMessage");
	}
};


UENUM(BlueprintType)
enum EDataTableContentIndex {
	FileSavedOK				= 0,
	FileSavedKO				= 1,
	FileLoadedOK			= 2,
	FileLoadedKO			= 3,
	FileLoadedKO404			= 4,
	FileDeletedOK			= 5,
	FileDeletedKO			= 6,
	PlacedTexturesDeleted	= 7,
	FilesAdded				= 8,
	FilesModified			= 9,
	FilesRemoved			= 10,
	MissingTexturesMessage	= 11,
	ScreenshotNameMessage	= 12,
	IncorrectScreenshotName	= 13,
	IncorrectPath			= 14,
	ScreenshotDirSetOK		= 15,
	ResourcesDirSetOK		= 16,
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "AMyController.h"


#include "DirectoryWatcherModule.h"
#include "ImageUtils.h"
#include "MyReferenceManager.h"
#include "MySaveGame.h"
#include "Engine/Texture.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "MDVProject4/Objects/AMyActor.h"
#include "EditorFramework/AssetImportData.h"
#include "MDVProject4/UI/HUD/MyHUD.h"
#include "MDVProject4/UI/Widgets/TileSelect.h"
#include "MDVProject4/Utils/Defines.h"


AMyController::AMyController() {
	// Set material
	BaseMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/Resources/BaseMaterial.BaseMaterial'"));
	MessageDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/DataTable/DT_UIMessages.DT_UIMessages'"));
	
	WallHovered = false;
}


void AMyController::BeginPlay() {
	Super::BeginPlay();
	MyReferenceManager = Cast<AMyReferenceManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyReferenceManager::StaticClass()));
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AMyActor::StaticClass(), WallsTag, MyWalls);
	MessageDataTableRowNames = MessageDataTable->GetRowNames();
	WatcherHandle = FDelegateHandle();

	LoadSettings();
	if (ResourcesDir.IsEmpty())	{
		//UE_LOG(LogTemp, Warning, TEXT("Resources Dir is empty"))
		ResourcesDir = FPaths::ProjectContentDir() + M_RESOURCES_DIR;
	}
	if (ScreenshotDir.IsEmpty()) {
		//TODO: is setting a default dir necessari? I believe it is set by default
		//UE_LOG(LogTemp, Warning, TEXT("Screenshot Dir is empty"))
	}
	InitialiseDynamicMaterialArray();
	RegisterDirectoryWatcherDelegate();
}

/**
 * Registers a delegate that will trigger OnProjectDirectoryChanged() when a change is performed on ResourcesDirPath
 */
void AMyController::RegisterDirectoryWatcherDelegate() {
	FDirectoryWatcherModule &DirectoryWatcherModule = FModuleManager::LoadModuleChecked<FDirectoryWatcherModule>(TEXT("DirectoryWatcher"));
	IDirectoryWatcher* DirectoryWatcher = DirectoryWatcherModule.Get();
	DirectoryWatcher->RegisterDirectoryChangedCallback_Handle(
		ResourcesDir,
		IDirectoryWatcher::FDirectoryChanged::CreateUObject(this, &AMyController::OnProjectDirectoryChanged),
		WatcherHandle,
		IDirectoryWatcher::WatchOptions::IncludeDirectoryChanges);
}

void AMyController::UnregisterDirectoryWatcherDelegate() {
	FDirectoryWatcherModule &DirectoryWatcherModule = FModuleManager::LoadModuleChecked<FDirectoryWatcherModule>(TEXT("DirectoryWatcher"));
	DirectoryWatcherModule.Get()->UnregisterDirectoryChangedCallback_Handle(ResourcesDir, WatcherHandle);
}

/**
 * Will trigger when a change (addition, replacement, deletion) is performed on ResourcesDirPath
 * @param Data Array of type FFileChangeData indicating the nature of the change observed
 */
void AMyController::OnProjectDirectoryChanged(const TArray<FFileChangeData>& Data) {
	for (FFileChangeData Element : Data) {
		switch (Element.Action) {
			case FFileChangeData::FCA_Added:
        		UpdateDynamicMaterialArray(Element.Filename, FFileChangeData::FCA_Added);
				//MyReferenceManager->MyHUD->AddTile();
				MyReferenceManager->MyHUD->Notify(Info, RetrieveDataTableMessage(FilesAdded));
				break;
			
        	case FFileChangeData::FCA_Modified:
        		UpdateDynamicMaterialArray(Element.Filename, FFileChangeData::FCA_Modified);
				//MyReferenceManager->MyHUD->RefreshTile();
				MyReferenceManager->MyHUD->Notify(Info, RetrieveDataTableMessage(FilesModified));
        		break;
			
        	case FFileChangeData::FCA_Removed:
        		UpdateDynamicMaterialArray(Element.Filename, FFileChangeData::FCA_Removed);
				//MyReferenceManager->MyHUD->RemoveTile();
				MyReferenceManager->MyHUD->Notify(Info, RetrieveDataTableMessage(FilesRemoved));
        		break;
			
	        default:;
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("Updating due to a modification on the files"))
	MyReferenceManager->MyHUD->RefreshTilesWidget(DynamicMaterialArray);
}

/**
 * Updates the MyDynamicMatArray based on the action performed by the directory watcher delegate
 * @param FileName Path to the file that requires to be handled
 * @param Action The specific action (ie. addition, replacement, deletion) that the file performed
 */
void AMyController::UpdateDynamicMaterialArray(const FString& FileName, FFileChangeData::EFileChangeAction Action) {
	switch (Action) {
		case FFileChangeData::FCA_Added: {
			InsertItemToDynamicMaterialArray(FileName);
		}
		break;
		
		case FFileChangeData::FCA_Modified: {
			for (FMyDynamicMat Element : DynamicMaterialArray) {
				if (Element.Path == FileName) {
					DynamicMaterialArray.Remove(Element);
					InsertItemToDynamicMaterialArray(FileName);
					break;
				}
			}
		}
		break;
		
		case FFileChangeData::FCA_Removed: {
			// Search for the removed material in the DynamicMaterialArray and remove it
			for (FMyDynamicMat Element : DynamicMaterialArray) {
				if (Element.Path == FileName) {
					// Search for any Wall that currently has the selected material and set it to the default one
					for (AActor* WallActor : MyWalls) {
						const AMyActor* MyWall = Cast<AMyActor>(WallActor);
						if (MyWall->StaticMesh->GetMaterial(M_MAT_NUM) == Cast<UMaterialInterface>(Element.DynamicMaterial)) {
							MyWall->StaticMesh->SetMaterial(M_MAT_NUM, MyWall->MaterialInterface);
							MyReferenceManager->MyHUD->Notify(Warning, RetrieveDataTableMessage(PlacedTexturesDeleted));
						}
					}
					DynamicMaterialArray.Remove(Element);
					break;
				}
			}
		}
		break;
		
		default: ;
	}
}

/**
 * Creates and populates MyDynamicMatArray with the .png and .jpg files found in "<ProjectDir>/Resources/TileResources/"
 */
void AMyController::InitialiseDynamicMaterialArray() {
	IFileManager& FileManager = IFileManager::Get();
	TArray<FString> FoundFiles;

	// Add to FoundFiles all the images from the hardcoded directory
	FileManager.FindFiles(FoundFiles, *ResourcesDir, *FString("png"));
	FileManager.FindFiles(FoundFiles, *ResourcesDir, *FString("jpg"));
	FileManager.FindFiles(FoundFiles, *ResourcesDir, *FString("jpeg"));

	for (FString FileName : FoundFiles) {
		const FString FilePath = ResourcesDir + "/" + FileName;
		InsertItemToDynamicMaterialArray(FilePath);
	}
}

/**
 * Adds a new FMyDynamicMat entry to MyDynamicMatArray
 * @param FilePath Source from where the new FMyDynamicMat entry is populated from
 */
void AMyController::InsertItemToDynamicMaterialArray(const FString& FilePath) {
	// Create Texture2D from image
	UTexture2D* Texture = UKismetRenderingLibrary::ImportFileAsTexture2D(nullptr, *FilePath);
	const FString BaseFileName = FPaths::GetCleanFilename(*FilePath);
	Texture->AssetImportData->AddFileName(UKismetSystemLibrary::ConvertToRelativePath(BaseFileName), 0);
	
	// Create dynamic material based on the previous texture
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, nullptr);
	DynamicMaterial->SetTextureParameterValue(FName("TextureParameter"), Texture);

	// Create, populate and store struct with the desired information
	FMyDynamicMat MyDynamicMatStruct;
	
	MyDynamicMatStruct.CleanName = FName(*BaseFileName);
	MyDynamicMatStruct.Path = *FilePath;
	MyDynamicMatStruct.Texture2D = Texture;
	MyDynamicMatStruct.DynamicMaterial = DynamicMaterial;
	
	DynamicMaterialArray.Add(MyDynamicMatStruct);
}

/**
 * Returns the StaticMeshComponent of the SelectedWall variable
 * @return The SelectedWall StaticMeshComponent 
 */
UStaticMeshComponent* AMyController::GetSelectedWallStaticMeshComponent() const {
	if (SelectedWall) {
		TArray<UStaticMeshComponent*> Components;
        		
		SelectedWall->GetComponents<UStaticMeshComponent>(Components);
		for(int i = 0; i < Components.Num(); i++) {
			UStaticMeshComponent* StaticMeshComponent = Components[i];
			return StaticMeshComponent;
		}
	}
	return nullptr;
}


/**
 * Updates the selected wall with the selected material
 * @param DynamicMaterial Dynamic material to set on the static mesh
 */
void AMyController::SetWallMaterial(UMaterialInstanceDynamic* DynamicMaterial) const {
	if (UStaticMeshComponent* StaticMeshComponent = GetSelectedWallStaticMeshComponent()) {
		StaticMeshComponent->SetMaterial(M_MAT_NUM, DynamicMaterial);
	}
}

/**
 * Sets the currently selected wall and updates the UI to display the tag name
 * @param MyWallActor Wall that has been clicked on
 */
void AMyController::UpdateSelectedWall(AMyActor* MyWallActor) {
	if (UStaticMeshComponent* StaticMeshComponent = GetSelectedWallStaticMeshComponent()) {
		StaticMeshComponent->SetRenderCustomDepth(false);
	}
	
	if (MyWallActor) {
		SelectedWall = MyWallActor;
		if (MyWallActor && !MyWallActor->Tags.IsEmpty() && MyWallActor->Tags.IsValidIndex(1)) {
			MyReferenceManager->MyHUD->UpdateSelectedWallText(MyWallActor);
			if (UStaticMeshComponent* StaticMeshComponent = GetSelectedWallStaticMeshComponent()) {
				StaticMeshComponent->SetRenderCustomDepth(true);
			}
		}
	}
}

/**
 * Updates the selected wall with its default material
 */
void AMyController::SetDefaultMaterial() const {
	if (UStaticMeshComponent* StaticMeshComponent = GetSelectedWallStaticMeshComponent()) {
		StaticMeshComponent->SetMaterial(M_MAT_NUM, SelectedWall->MaterialInterface);
	}
}

void AMyController::SaveSettings() {
	UMySaveGame* MySaveGame = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	MySaveGame->ResourcesDir = ResourcesDir;
	MySaveGame->ScreenshotDir = ScreenshotDir;
	UGameplayStatics::SaveGameToSlot(MySaveGame, M_SAVE_SLOT_NAME, M_SAVE_SLOT_NUM);
}

void AMyController::LoadSettings() {
	if (UGameplayStatics::DoesSaveGameExist(M_SAVE_SLOT_NAME, M_SAVE_SLOT_NUM)) {
		const UMySaveGame* MySaveGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(M_SAVE_SLOT_NAME, M_SAVE_SLOT_NUM));
		ResourcesDir = MySaveGame->ResourcesDir;
		ScreenshotDir = MySaveGame->ScreenshotDir;
	}
}

/**
 * Saves the specified information from the game
 */
void AMyController::SaveGame() {
	UMySaveGame* MySaveGame = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	for (AActor* Element : MyWalls) {
		// Get Wall
		AMyActor* Wall = Cast<AMyActor>(Element);
		// Get texture name used in this wall
		const UMaterialInterface* WallMaterialInterface = Wall->StaticMesh->GetMaterial(M_MAT_NUM);
		TArray<UTexture*> UsedTextures;
		WallMaterialInterface->GetUsedTextures(UsedTextures, EMaterialQualityLevel::Num, true, ERHIFeatureLevel::SM5, true);
		FString CleanTextureSourceFileName = FPaths::GetCleanFilename(UsedTextures[0]->AssetImportData->GetFirstFilename());
		// Create new Map pair
		TPair<AMyActor*, FString> Pair;
		Pair.Key = Wall;
		Pair.Value = CleanTextureSourceFileName;
		
		MySaveGame->SaveMap.Add(Pair);
	}
	
	UGameplayStatics::SaveGameToSlot(MySaveGame, M_SAVE_SLOT_NAME, M_SAVE_SLOT_NUM);
	const FDataTableContent* DataTableContent = MessageDataTable->FindRow<FDataTableContent>(MessageDataTableRowNames[FileSavedOK], "", true);
	MyReferenceManager->MyHUD->Notify(Info, DataTableContent->Message);
}

/**
 * Load the specified information from the game
 */
void AMyController::LoadGame() {
	if (UGameplayStatics::DoesSaveGameExist(M_SAVE_SLOT_NAME, M_SAVE_SLOT_NUM)) {
		const UMySaveGame* MySaveGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(M_SAVE_SLOT_NAME, M_SAVE_SLOT_NUM));
		SaveMap = MySaveGame->SaveMap;
		
		if (RenderSaveMap()) {
			MyReferenceManager->MyHUD->Notify(Info, RetrieveDataTableMessage(FileLoadedOK));
		}
	} else {
		MyReferenceManager->MyHUD->Notify(Error, RetrieveDataTableMessage(FileLoadedKO404));
	}
}

/**
 * Iterates over the TMap save file to assign materials to walls, if the material's source file exist.
 */
bool AMyController::RenderSaveMap() {
	TMap<FString, FString> MissingFiles;
	for (const TPair<AMyActor*, FString>& SaveMapEntry : SaveMap) {
		// Check if the wall material is the default base material
		if (SaveMapEntry.Value == M_BASE_TEXTURE_NAME) {
			SaveMapEntry.Key->StaticMesh->SetMaterial(M_MAT_NUM, SaveMapEntry.Key->MaterialInterface);
			continue;
		}

		// If the wall's material exists, look for it in the DynamicMaterialArray and set it 
		if (FPaths::FileExists(ResourcesDir + "/" + SaveMapEntry.Value)) {
			for (FMyDynamicMat DynamicMat : DynamicMaterialArray) {
				if (DynamicMat.CleanName == SaveMapEntry.Value) {
					SaveMapEntry.Key->StaticMesh->SetMaterial(M_MAT_NUM, DynamicMat.DynamicMaterial);
					break;
				}
			}
		} else {
			TPair<FString, FString> Pair;
			Pair.Key = SaveMapEntry.Key->Tags[1].ToString();
			Pair.Value = SaveMapEntry.Value;
			
			MissingFiles.Add(Pair);
			SaveMapEntry.Key->StaticMesh->SetMaterial(M_MAT_NUM, SaveMapEntry.Key->MaterialInterface);
		}
	}
	if (!MissingFiles.IsEmpty()) {
		for (auto MissingFile : MissingFiles) {
			//UE_LOG(LogTemp, Warning, TEXT("Missing the following texture: %s"), *MissingFile.Value)
		}
		
		MyReferenceManager->MyHUD->DisplayMissingFilesDialog(MissingFiles);
		return false;
	}
	return true;
}

/**
 * Deletes the save file
 */
void AMyController::DeleteSaveFile() {
	if (UGameplayStatics::DoesSaveGameExist(M_SAVE_SLOT_NAME, M_SAVE_SLOT_NUM)) {
		if (UGameplayStatics::DeleteGameInSlot(M_SAVE_SLOT_NAME, M_SAVE_SLOT_NUM)) {
			MyReferenceManager->MyHUD->Notify(Info, RetrieveDataTableMessage(FileDeletedOK));
		} else {
			MyReferenceManager->MyHUD->Notify(Error, RetrieveDataTableMessage(FileDeletedKO));
		}
	}
}

/**
 * Handles any click on the screen to clear the selected wall label and remove the wall's outline
 */
void AMyController::ScreenClicked() {
	if (!WallHovered) {
		if (UStaticMeshComponent* StaticMeshComponent = GetSelectedWallStaticMeshComponent()) {
			StaticMeshComponent->SetRenderCustomDepth(false);
			MyReferenceManager->MyHUD->UpdateSelectedWallText(nullptr);
		}
		SelectedWall = nullptr;
	} 
}

/**
 * Requests the creation of a screenshot
 * @param ScreenshotName Name of the screenshot provided by the user via the UI
 */
void AMyController::CreateScreenshot(const FText& ScreenshotName) const {
	if (UStaticMeshComponent* StaticMeshComponent = GetSelectedWallStaticMeshComponent()) {
		StaticMeshComponent->SetRenderCustomDepth(false);
		MyReferenceManager->MyHUD->UpdateSelectedWallText(nullptr);
	}
	// Store screenshot in Project directory next to main UProject/EXE based on the build type
	#if WITH_EDITOR
		const FString ImageDirectory = FString::Printf(TEXT("%s/%s"), *FPaths::ProjectDir(), TEXT("Screenshots"));
	#else
		const FString ImageDirectory = FString::Printf(TEXT("%s/../%s"), *FPaths::ProjectDir(), TEXT("Screenshots"));
	#endif
	ScreenshotFilename = FString::Printf(TEXT("%s/%s.png"), *ScreenshotDir, *ScreenshotName.ToString());
	if (!ScreenshotDir.IsEmpty() && FPaths::ValidatePath(ScreenshotFilename) && FPaths::DirectoryExists(ScreenshotDir)) {
		FScreenshotRequest::RequestScreenshot(ScreenshotFilename, false, false);
		UGameViewportClient::OnScreenshotCaptured().AddStatic(&ScreenshotCaptured);
	} else {
		MyReferenceManager->MyHUD->Notify(Error, MyReferenceManager->MyHUD->RetrieveDataTableMessage(IncorrectPath));	
	}
}

/**
 * Called via delegate once the request to create a screenshot has been carried out
 * @param Width Screenshot width
 * @param Height Screenshot height
 * @param Bitmap Array of bits that specify the color of each pixel in a rectangular array of pixels
 */
void AMyController::ScreenshotCaptured(int32 Width, int32 Height, const TArray<FColor>& Bitmap) {
	// Create .png from Bitmap
	TArray64<uint8> ImageData;
	TArray BitmapCopy(Bitmap);
	FImageUtils::PNGCompressImageArray(Width, Height, BitmapCopy, ImageData);
	
	if (FFileHelper::SaveArrayToFile(ImageData, *ScreenshotFilename)) {
		//UE_LOG(LogTemp, Warning, TEXT("true"));
	} else {
		//TODO: When can this operation fail? Display generic error msg for the user
		//UE_LOG(LogTemp, Warning, TEXT("false"));
	}
	

	// Create Texture2D from Bitmap
	UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height);
	if (Texture) {
		Texture->CompressionSettings = TC_Displacementmap;
		Texture->SRGB = 0;

		FTexture2DMipMap& MipMap = Texture->GetPlatformData()->Mips[0];

		void* Data = MipMap.BulkData.Lock(LOCK_READ_WRITE);

		constexpr int32 Stride = sizeof(uint8) * 4;
		FMemory::Memcpy(Data, Bitmap.GetData(), Texture->GetPlatformData()->SizeX * Texture->GetPlatformData()->SizeY * Stride);
		MipMap.BulkData.Unlock();
		
		Texture->UpdateResource();
		MyReferenceManager->MyHUD->TriggerScreenshotEffect(Texture);
	}
}

/**
 * Returns the message from MessageDataTable belonging to the DataTableContentIndex used
 * @param DataTableContentIndex EDataTableContentIndex numerical index used to indicate a MessageDataTable row
 * @return The MessageDataTable row's message
 */
FText AMyController::RetrieveDataTableMessage(const EDataTableContentIndex DataTableContentIndex) {
	return MessageDataTable->FindRow<FDataTableContent>(MessageDataTableRowNames[DataTableContentIndex], "", true)->Message;
}

void AMyController::UpdateResourcesDir(FString FolderDir) {
	UnregisterDirectoryWatcherDelegate();
	DynamicMaterialArray.Empty();
	MyReferenceManager->MyHUD->RefreshTilesWidget(DynamicMaterialArray);
	MyReferenceManager->MyHUD->Notify(Info, RetrieveDataTableMessage(FilesModified));
	ResourcesDir = FolderDir;
	SaveSettings();
	InitialiseDynamicMaterialArray();
	MyReferenceManager->MyHUD->RefreshTilesWidget(DynamicMaterialArray);
	RegisterDirectoryWatcherDelegate();
}

void AMyController::UpdateScreenshotDir(FString FolderDir){
	ScreenshotDir = FolderDir;
	SaveSettings();
}

void AMyController::DebugPressed() {
	ResourcesDir = nullptr;
	ScreenshotDir = nullptr;
}

bool AMyController::IsTileSelectEnabled() {
	return MyReferenceManager->MyHUD->IsTileSelectEnabled();
}

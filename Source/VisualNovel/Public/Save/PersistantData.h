#pragma once
#include "VNStruct.h"
#include "GameFramework/SaveGame.h"
#include "PersistantData.generated.h"

UCLASS()
class VISUALNOVEL_API UPersistantData : public USaveGame
{
	GENERATED_BODY()
public:
	UPersistantData();
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TMap<FString, FStringArray> mUnlockedGalleryImgs;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	float mTextSpeed;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	bool bShowUnselectableOption;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	bool bShowPreviouslyPickedChoices;


	UPROPERTY(BlueprintReadWrite, Category = "Skip")
	float mSkipSpeed;
	UPROPERTY(BlueprintReadWrite, Category = "Skip")
	bool bCancelSkipOnOptions;

	UPROPERTY(BlueprintReadWrite, Category = "Auto")
	float mAutoSpeed;
	UPROPERTY(BlueprintReadWrite, Category = "Auto")
	bool bCancelAutoOnOptions;

	UPROPERTY(BlueprintReadWrite, Category = "SaveLoad")
	TArray<FString> mSaveNames;

	UPROPERTY(BlueprintReadWrite, Category = "Sound")
	TArray<float> mVolumes;
};

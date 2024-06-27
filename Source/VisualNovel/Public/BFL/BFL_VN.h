#pragma once
#include "VNStruct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_VN.generated.h"

class UDlgDialogue;

UCLASS()
class VISUALNOVEL_API UBFL_VN : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UBFL_VN();

protected:
	static TObjectPtr<UStringTable> mKeywordData;
	static TObjectPtr<UStringTable> mStyleData;
	static TObjectPtr<UDataTable> mParticipantData;
	static TObjectPtr<USoundMix> mSoundMix;

public:
	static FString mRecentSlotName;

public:
	UFUNCTION(BlueprintPure, Category = "VisualNovel")
	static bool GetKeyword(FString InText,FString& OutText);
	UFUNCTION(BlueprintPure, Category = "VisualNovel")
	static bool GetStyleword(FString InText, FString& OutText);
	UFUNCTION(BlueprintPure, Category = "VisualNovel")
	static FParticipantData GetParticipantData(EParticipantName Name);

	UFUNCTION(BlueprintPure, Category = "VisualNovel")
	static FString ToTargetString(FText InText,bool AddQuotes=false,
		int32 TextSize = 28);
	UFUNCTION(BlueprintPure, Category = "VisualNovel")
	static FText ToTargetText(FText InText,bool AddQuotes=false,
		int32 TextSize = 28);
	UFUNCTION(BlueprintPure, Category = "VisualNovel")
	static FString RemoveSymbolText(FString InText);

	UFUNCTION(BlueprintCallable, Category = "UIScreenshot")
	static void TakeScreenshotOfUI(FString Name);

	UFUNCTION(BlueprintCallable, Category = TDLHelpers)
	static void GetAllSaveGameSlotNames(TArray<FString>& Names);

	UFUNCTION(BlueprintPure, Category = "Sound")
	static USoundMix* GetVNSoundMix()
	{
		return mSoundMix;
	}
	UFUNCTION(BlueprintCallable, Category = "Sound")
	static void SetVolume(const UObject* WorldContextObject,float Value, 
		ESoundKind Sound);
};

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
	static TObjectPtr<UDataTable> mParticipantData;
	static TObjectPtr<UDataTable> mBGImgData;
public:
	static TObjectPtr<UDlgDialogue> mDialogue;

public:
	UFUNCTION(BlueprintPure, Category = "VisualNovel")
	static bool GetKeyword(FString InText,FString& OutText);
	UFUNCTION(BlueprintPure, Category = "VisualNovel")
	static FParticipantData GetParticipantData(EParticipantName Name);
	UFUNCTION(BlueprintPure, Category = "VisualNovel")
	static FBGImgData GetBGImgData(FName Name);

	UFUNCTION(BlueprintPure, Category = "VisualNovel")
	static FString ToTargetString(FText InText,bool AddQuotes=false);
	UFUNCTION(BlueprintPure, Category = "VisualNovel")
	static FText ToTargetText(FText InText,bool AddQuotes=false);
	UFUNCTION(BlueprintPure, Category = "VisualNovel")
	static FString RemoveSymbolText(FString InText);
	UFUNCTION(BlueprintPure, Category = "VisualNovel")
	static float FakeLerp(float CurValue,float TargetValue,float DelataTime,float Speed);
};

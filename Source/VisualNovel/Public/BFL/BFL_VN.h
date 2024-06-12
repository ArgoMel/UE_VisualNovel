#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_VN.generated.h"

UCLASS()
class VISUALNOVEL_API UBFL_VN : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UBFL_VN();
protected:
	static TObjectPtr<UStringTable> mKeywordData;

public:
	UFUNCTION(BlueprintPure, Category = "VisualNovel")
	static bool GetKeyword(FString InText,FString& OutText);

	UFUNCTION(BlueprintPure, Category = "VisualNovel")
	static FString ToTargetText(FText InText,bool AddQuotes=false);

	UFUNCTION(BlueprintPure, Category = "VisualNovel")
	static FString RemoveSymbolText(FString InText);
};

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_VN.generated.h"

UCLASS()
class VISUALNOVEL_API UBFL_VN : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "VisualNovel")
	static FString ToTargetText(FText InText,bool AddQuotes);
};

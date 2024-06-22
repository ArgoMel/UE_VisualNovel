#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_VNSave.generated.h"

class USG_VN;

UINTERFACE(MinimalAPI)
class UInterface_VNSave : public UInterface
{
	GENERATED_BODY()
};

class VISUALNOVEL_API IInterface_VNSave
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VN")
	void OnNewGame();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VN")
	void OnSaveGame(USG_VN* SaveGame);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VN")
	void OnLoadGame(USG_VN* SaveGame);
};

#pragma once
#include "CoreMinimal.h"
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
	float mTextSpeed;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	bool bShowUnselectableOption;

	UPROPERTY(BlueprintReadWrite, Category = "Skip")
	float mSkipSpeed;
	UPROPERTY(BlueprintReadWrite, Category = "Skip")
	bool bCancelSkipOnOptions;

	UPROPERTY(BlueprintReadWrite, Category = "Auto")
	float mAutoSpeed;
	UPROPERTY(BlueprintReadWrite, Category = "Auto")
	bool bCancelAutoOnOptions;
};

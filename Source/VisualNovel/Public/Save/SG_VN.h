#pragma once
#include "VNStruct.h"
#include "GameFramework/SaveGame.h"
#include "SG_VN.generated.h"

UCLASS()
class VISUALNOVEL_API USG_VN : public USaveGame
{
	GENERATED_BODY()
public:
	USG_VN();

public:
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	TSet<int32> mVisitedNodes;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	FName mCurDialogueName;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	FName mCurBG;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	int32 mActiveNodeIndex;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	bool bAskForPlayerName;

	UPROPERTY(BlueprintReadWrite, Category = "History")
	TArray<FString> mHistory;

	UPROPERTY(BlueprintReadWrite, Category = "Participant")
	TMap<FName, FCharacterSaveInfo> mCharacterInfo;

	UPROPERTY(BlueprintReadWrite, Category = "GameInstance")
	TSet<FName> mTriggeredFlags;
	UPROPERTY(BlueprintReadWrite, Category = "GameInstance")
	FDateTime mSaveTime;
	UPROPERTY(BlueprintReadWrite, Category = "GameInstance")
	FName mPlayerName;
};

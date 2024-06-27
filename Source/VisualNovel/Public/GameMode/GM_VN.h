#pragma once
#include "VNStruct.h"
#include "GameFramework/GameModeBase.h"
#include "GM_VN.generated.h"

class UAudioComponent;
class UGI_VN;
class AVNSceneCapture2D;

UCLASS()
class VISUALNOVEL_API AGM_VN : public AGameModeBase
{
	GENERATED_BODY()
public:
	AGM_VN();
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UAudioComponent> mBGMComp;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UAudioComponent> mVoiceComp;

	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UGI_VN> mGameInstance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Variable")
	TMap<FName, AVNSceneCapture2D*> mVNSceneCaptures;

public:
	UMaterialInstance* GetSceneCaptureMatByName(FName TexName, FName OldName);
	void SetBGMByName(FName BGMName);
	void SetVoice(USoundBase* Voice);
};

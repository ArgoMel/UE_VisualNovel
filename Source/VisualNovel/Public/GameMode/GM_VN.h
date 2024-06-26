#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_VN.generated.h"

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
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Variable")
	TMap<FName, AVNSceneCapture2D*> mVNSceneCaptures;

public:
	UMaterialInstance* GetSceneCaptureMatByName(FName TexName, FName OldName);
};

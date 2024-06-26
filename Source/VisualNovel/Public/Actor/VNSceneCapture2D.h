#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VNSceneCapture2D.generated.h"

class USceneCaptureComponent2D;
class UCameraComponent;

UCLASS()
class VISUALNOVEL_API AVNSceneCapture2D : public AActor
{
	GENERATED_BODY()	
public:	
	AVNSceneCapture2D();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, Category = "Component")
	TObjectPtr<USceneCaptureComponent2D> mSceneCaptureComp;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UCameraComponent> mCameraComp;

private:
	TObjectPtr<UMaterialInterface> mSceneCaptureMat;

protected:
	TObjectPtr<UMaterialInstance> mSceneCaptureMI;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Variable")
	FName mBGName;

public:
	void ToggleCaptureEveryFrame();

	UMaterialInstance* GetSceneCaptureMI()
	{
		return mSceneCaptureMI;
	}
};

#pragma once
#include "CoreMinimal.h"
#include "PlayerController/PC_Input.h"
#include "PC_VN.generated.h"

class UGI_VN;

UCLASS()
class VISUALNOVEL_API APC_VN : public APC_Input
{
	GENERATED_BODY()
public:
	APC_VN();
protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UGI_VN> mGameInstance;

protected:
	UFUNCTION()
	void ToggleGameAndMenu();
};

#pragma once
#include "CoreMinimal.h"
#include "Input/VNInput.h"
#include "GameFramework/PlayerController.h"
#include "PC_Input.generated.h"

UCLASS()
class VISUALNOVEL_API APC_Input : public APlayerController
{
	GENERATED_BODY()
public:
	APC_Input();
protected:
	virtual void SetupInputComponent() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UVNInput> mVNInputAsset;

private:
	UFUNCTION()
	void AddInputMapping(const UInputMappingContext* InputMapping, const int32 MappingPriority = 0) const;
	UFUNCTION()
	void RemoveInputMapping(const UInputMappingContext* InputMapping) const;

public:
	UFUNCTION()
	void SetInput(const UInputMappingContext* InputMapping, const bool Enabled = true) const;

	UFUNCTION()
	UVNInput* GetVNInputAsset() const
	{
		return mVNInputAsset;
	}
};

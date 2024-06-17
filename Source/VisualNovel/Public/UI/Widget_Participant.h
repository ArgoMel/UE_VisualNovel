#pragma once
#include "VNStruct.h"
#include "Blueprint/UserWidget.h"
#include <DlgSystem/DlgDialogueParticipant.h>
#include "Widget_Participant.generated.h"

class UImage;

UCLASS(Abstract)
class VISUALNOVEL_API UWidget_Participant : public UUserWidget
	, public IDlgDialogueParticipant
{
	GENERATED_BODY()
public:
	UWidget_Participant(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
public:
	FName GetParticipantName_Implementation() const;
	FText GetParticipantDisplayName_Implementation(FName ActiveSpeaker) const;
	bool ModifyNameValue_Implementation(FName ValueName, FName NameValue);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> CharacterImg;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> JumpAnim;

	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	FParticipantData mParticipantData;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	FName mCharacterName;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variable")
	EParticipantName mParticipantName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category = "Variable")
	double mTargetX;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category = "Variable")
	double mMoveSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Variable")
	double mTargetOpacity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Variable")
	double mFadeSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Variable")
	bool bWasIntroduced;

protected:
	UFUNCTION()
	void Jump();
	UFUNCTION()
	void ToggleOpacity();

public:
	UFUNCTION(BlueprintCallable, Category = "Event")
	void FinishAnimating();

	FName GetParticipantName()
	{
		return mCharacterName;
	}
};

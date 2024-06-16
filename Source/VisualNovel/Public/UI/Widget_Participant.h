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

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> CharacterImg;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> JumpAnim;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variable")
	EParticipantName mParticipantName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variable")
	FVector2D mCharacterAlignment;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Variable")
	float mCharacterScale;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	double mTargetX;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	double mMoveSpeed;

protected:
	UFUNCTION()
	void TestFunc();
	UFUNCTION()
	void Jump();

public:
	FName GetParticipantName()
	{
		return FName(UEnum::GetDisplayValueAsText(mParticipantName).ToString());
	}
};

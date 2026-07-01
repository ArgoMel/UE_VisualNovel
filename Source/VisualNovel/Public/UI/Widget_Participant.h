#pragma once
#include "VNStruct.h"
#include "Blueprint/UserWidget.h"
#include <DlgSystem/DlgDialogueParticipant.h>
#include "Interface/Interface_VNSave.h"
#include "Widget_Participant.generated.h"

class UImage;
class USG_VN;

UCLASS(Abstract)
class VISUALNOVEL_API UWidget_Participant : public UUserWidget
	, public IDlgDialogueParticipant
	, public IInterface_VNSave
{
	GENERATED_BODY()
public:
	explicit UWidget_Participant(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	virtual FName GetParticipantName_Implementation() const override;
	virtual FText GetParticipantDisplayName_Implementation(FName ActiveSpeaker) const override;
	virtual bool ModifyNameValue_Implementation(FName ValueName, FName NameValue) override;
public:
	virtual void OnNewGame_Implementation() override;
	virtual void OnSaveGame_Implementation(USG_VN* SaveGame) override;
	virtual void OnLoadGame_Implementation(USG_VN* SaveGame) override;

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

	void SetParticipantData(EParticipantName ParticipantName);

public:
	UFUNCTION(BlueprintCallable, Category = "Event")
	void FinishAnimating();

	FName GetParticipantName()
	{
		return mCharacterName;
	}
};

#include "UI/Widget_Participant.h"
#include "BFL/BFL_VN.h"
#include "../VisualNovel.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

UWidget_Participant::UWidget_Participant(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	mParticipantName = EParticipantName::Yuri;

	mCharacterAlignment = FVector2D(0.5,0.65);
	mCharacterScale = 0.6f;
	mTargetX = 0.;
	mMoveSpeed = 5000.;
}

void UWidget_Participant::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UWidget_Participant::NativePreConstruct()
{
	Super::NativePreConstruct();
	FParticipantData data= UBFL_VN::GetParticipantData(mParticipantName);
	CharacterImg->SetBrushFromTexture(data.CharacterTexture);
	UCanvasPanelSlot* slot=Cast<UCanvasPanelSlot>(CharacterImg->Slot);
	if(IsValid(slot))
	{
		slot->SetOffsets(FMargin(0.,0., 
			data.CharacterTexture->GetSizeX()* mCharacterScale, 
			data.CharacterTexture->GetSizeY()* mCharacterScale));
		slot->SetAlignment(mCharacterAlignment);
	}
}

void UWidget_Participant::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	FVector2D translation= CharacterImg->GetRenderTransform().Translation;
	if(FMath::IsNearlyEqual(translation.X,mTargetX))
	{
		return;
	}
	if(translation.X<mTargetX)
	{
		translation.X = FMath::Min(mMoveSpeed* InDeltaTime+ translation.X, mTargetX);
	}
	else
	{
		translation.X = FMath::Max(-mMoveSpeed* InDeltaTime+ translation.X, mTargetX);
	}
	CharacterImg->SetRenderTranslation(translation);
}

FName UWidget_Participant::GetParticipantName_Implementation() const
{
	return FName(UEnum::GetDisplayValueAsText(mParticipantName).ToString());
}

FText UWidget_Participant::GetParticipantDisplayName_Implementation(FName ActiveSpeaker) const
{
	return UEnum::GetDisplayValueAsText(mParticipantName);
}

void UWidget_Participant::TestFunc()
{
	FVector2D translation = CharacterImg->GetRenderTransform().Translation;
	translation.X = mTargetX;
	CharacterImg->SetRenderTranslation(translation);
}

void UWidget_Participant::Jump()
{
	PlayAnimation(JumpAnim);
}

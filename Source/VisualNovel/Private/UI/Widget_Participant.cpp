#include "UI/Widget_Participant.h"
#include "BFL/BFL_VN.h"
#include "../VisualNovel.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

UWidget_Participant::UWidget_Participant(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	mParticipantName = EParticipantName::Yuri;

	mTargetX = 0.;
	mMoveSpeed = 2000.;
	mTargetOpacity = 1.;
	mFadeSpeed = 4.;
	bWasIntroduced = false;
}

void UWidget_Participant::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mCharacterName = FName(UEnum::GetDisplayValueAsText(mParticipantName).ToString());
}

void UWidget_Participant::NativePreConstruct()
{
	Super::NativePreConstruct();
	mCharacterName = FName(UEnum::GetDisplayValueAsText(mParticipantName).ToString());
	mParticipantData = UBFL_VN::GetParticipantData(mParticipantName);
	CharacterImg->SetBrushFromTexture(mParticipantData.Texture);
	UCanvasPanelSlot* slot=Cast<UCanvasPanelSlot>(CharacterImg->Slot);
	if(IsValid(slot))
	{
		slot->SetOffsets(FMargin(0.,0., 
			mParticipantData.Texture->GetSizeX()* mParticipantData.Scale,
			mParticipantData.Texture->GetSizeY()* mParticipantData.Scale));
		slot->SetAlignment(mParticipantData.Alignment);
	}
}

void UWidget_Participant::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	float opacity = FMath::FInterpConstantTo(
		CharacterImg->GetRenderOpacity(), mTargetOpacity, InDeltaTime, mFadeSpeed);
	CharacterImg->SetRenderOpacity(opacity);

	FVector2D translation = CharacterImg->GetRenderTransform().Translation;
	translation.X= FMath::FInterpConstantTo(translation.X,mTargetX,InDeltaTime,mMoveSpeed);
	CharacterImg->SetRenderTranslation(translation);
}

FName UWidget_Participant::GetParticipantName_Implementation() const
{
	return mCharacterName;
}

FText UWidget_Participant::GetParticipantDisplayName_Implementation(FName ActiveSpeaker) const
{
	if(!bWasIntroduced)
	{
		return FText::FromString(PARTICIPANTNAME_UNKNOWN);
	}
	return FText::FromName(mCharacterName);
}

bool UWidget_Participant::ModifyNameValue_Implementation(FName ValueName, FName NameValue)
{
	if(ValueName==TEXT("PositionX"))
	{
		
	}
	else if (ValueName == TEXT("Expression"))
	{
		if(mParticipantData.Expressions.Contains(NameValue))
		{
			CharacterImg->SetBrushFromTexture(mParticipantData.Expressions.FindRef(NameValue));
		}
	}
	return false;
}

void UWidget_Participant::Jump()
{
	PlayAnimation(JumpAnim);
}

void UWidget_Participant::ToggleOpacity()
{
	if(CharacterImg->GetRenderOpacity()==1.f)
	{
		mTargetOpacity = 0.f;
	}
	else
	{
		mTargetOpacity = 1.f;
	}
}

void UWidget_Participant::FinishAnimating()
{
	FVector2D translation = CharacterImg->GetRenderTransform().Translation;
	translation.X = mTargetX;
	CharacterImg->SetRenderTranslation(translation);
	StopAllAnimations();
}

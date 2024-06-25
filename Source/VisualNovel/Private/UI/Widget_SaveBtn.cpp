#include "UI/Widget_SaveBtn.h"
#include "GameInstance/GI_VN.h"
#include "Save/SG_VN.h"
#include "BFL/BFL_VN.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"

UWidget_SaveBtn::UWidget_SaveBtn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UWidget_SaveBtn::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mGameInstance = Cast<UGI_VN>(GetGameInstance());
	SaveBtn->OnClicked.AddDynamic(this,&ThisClass::OnSaveBtnClicked);
	SaveName->OnTextCommitted.AddDynamic(this,&ThisClass::OnSaveNameCommitted);
}

void UWidget_SaveBtn::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_SaveBtn::SetIsEnabled(bool bInIsEnabled)
{
	if (!bInIsEnabled)
	{
		Super::SetIsEnabled(SaveDetail->GetText().ToString() != TEXT("비어있음"));
	}
	else
	{
		Super::SetIsEnabled(true);
	}
}

void UWidget_SaveBtn::OnSaveBtnClicked()
{
	OnSaveOrLoad.Broadcast(mSlotName);
}

void UWidget_SaveBtn::OnSaveNameCommitted(const FText& Text, 
	ETextCommit::Type CommitMethod)
{
	if (CommitMethod != ETextCommit::OnEnter ||
		Text.IsEmpty())
	{
		SaveName->SetText(FText::FromString(mSlotName));
		return;
	}
	mGameInstance->ChangeVNSaveData(mSlotName, Text.ToString());
	mSlotName = Text.ToString();
}

void UWidget_SaveBtn::SetSaveImg(UTexture2D* Texture)
{
	SaveImg->SetBrushFromTexture(Texture);
}

void UWidget_SaveBtn::SetSaveName(FString Text)
{
	mSlotName = Text;
	SaveName->SetText(FText::FromString(Text));
	if(Text== SLOTNAME_QUICK_SAVE)
	{
		SaveName->SetIsEnabled(false);
	}
}

void UWidget_SaveBtn::SetSaveDetail(FText Text)
{
	SaveDetail->SetText(Text);
}

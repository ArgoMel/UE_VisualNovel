#include "GameInstance/GI_VN.h"
#include "UI/Widget_Menu.h"
#include "UI/Widget_Dialogue.h"
#include "UI/Widget_Option.h"
#include "UI/Widget_Codex.h"
#include "UI/Widget_CodexBtn.h"
#include "Save/PersistantData.h"
#include "../VisualNovel.h"
#include <Kismet/GameplayStatics.h>

UGI_VN::UGI_VN()
{
	GetClassAsset(mMenuWidgetClass, UUserWidget, "/Game/VN/UI/WP_Menu.WP_Menu_C");
	GetClassAsset(mDialogueWidgetClass, UUserWidget, "/Game/VN/UI/WP_Dialogue.WP_Dialogue_C");
}

void UGI_VN::Init()
{
	Super::Init();
	if (UGameplayStatics::DoesSaveGameExist(SLOTNAME_PERSISTANTDATA, 0))
	{
		mPersistantData = Cast<UPersistantData>(
			UGameplayStatics::LoadGameFromSlot(SLOTNAME_PERSISTANTDATA, 0));
	}
	else
	{
		mPersistantData = Cast<UPersistantData>(
			UGameplayStatics::CreateSaveGameObject(UPersistantData::StaticClass()));
	}
}

void UGI_VN::Shutdown()
{
	Super::Shutdown();
	UGameplayStatics::SaveGameToSlot(mPersistantData, SLOTNAME_PERSISTANTDATA,0);
}

FName UGI_VN::GetParticipantName_Implementation() const
{
	return PARTICIPANTNAME_GAMEINSTANCE;
}

bool UGI_VN::CheckCondition_Implementation(const UDlgContext* Context, 
	FName ConditionName) const
{
	return mTriggeredFlags.Contains(ConditionName);
}

bool UGI_VN::OnDialogueEvent_Implementation(UDlgContext* Context, FName EventName)
{
	mTriggeredFlags.AddUnique(EventName);
	return false;
}

bool UGI_VN::ModifyNameValue_Implementation(FName ValueName, FName NameValue)
{
	if(ValueName.ToString().StartsWith(TEXT("CDX_")))
	{
		if(!mTriggeredFlags.Contains(NameValue))
		{
			mTriggeredFlags.Add(NameValue);
			UWidget_Codex* codex = mMenuWidget->GetCodex();
			UWidget_CodexBtn* codexBtn = codex->mCodexBtns.FindRef(ValueName);
			codexBtn->UpdateCodexDetails();
			FString notify = FString::Printf(TEXT("%s 정보 갱신!"), *codexBtn->GetCodexText().ToString());
			mDialogueWidget->Notify(FText::FromString(notify));
		}
	}
	return false;
}

void UGI_VN::ShowMenu()
{
	mMenuWidget->AddToViewport(1);
}

void UGI_VN::ShowDialogue()
{
	mDialogueWidget->AddToViewport(0);
}

void UGI_VN::CreateMenu()
{
	if (!IsValid(mDialogueWidgetClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("UGI_VN::mDialogueWidgetClass 클래스 없음"));
		return;
	}
	if (IsValid(mDialogueWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("UGI_VN::mDialogueWidget 이미 생성"));
		return;
	}
	mDialogueWidget = CreateWidget<UWidget_Dialogue>(
		UGameplayStatics::GetPlayerController(GetWorld(), 0), mDialogueWidgetClass);

	if (!IsValid(mMenuWidgetClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("UGI_VN::mMenuWidgetClass 클래스 없음"));
		return;
	}
	if (IsValid(mMenuWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("UGI_VN::mMenuWidget 이미 생성"));
		return;
	}
	mMenuWidget = CreateWidget<UWidget_Menu>(
		UGameplayStatics::GetPlayerController(GetWorld(), 0), mMenuWidgetClass);
	mMenuWidget->Init(mDialogueWidget);
	mOptionWidget = mMenuWidget->GetOption();
	mOptionWidget->Init(mPersistantData);
	mOptionWidget->InitializeSavedOptions();
}

void UGI_VN::ToggleGameAndMenu()
{
	mMenuWidget->ToggleMenuWidget();
}

#include "GameInstance/GI_VN.h"
#include "UI/Widget_Menu.h"
#include "UI/Widget_Dialogue.h"
#include "UI/Widget_Option.h"
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

void UGI_VN::ShowMenu()
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

	mMenuWidget->AddToViewport(1);
}

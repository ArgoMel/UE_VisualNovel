#include "UI/Widget_Menu.h"
#include "UI/Widget_Dialogue.h"
#include "UI/Widget_History.h"
#include "BFL/BFL_VN.h"
#include "../VisualNovel.h"
#include "DlgSystem/DlgContext.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include <Kismet/KismetSystemLibrary.h>

UWidget_Menu::UWidget_Menu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetClassAsset(mDialogueWidgetClass, UUserWidget, "/Game/VN/UI/WP_Dialogue.WP_Dialogue_C");
}

void UWidget_Menu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	StartBtn->OnClicked.AddDynamic(this, &ThisClass::OnStartBtnClicked);
	SaveBtn->OnClicked.AddDynamic(this, &ThisClass::OnQuitBtnClicked);
	LoadBtn->OnClicked.AddDynamic(this, &ThisClass::OnQuitBtnClicked);
	CodexBtn->OnClicked.AddDynamic(this, &ThisClass::OnQuitBtnClicked);
	HistoryBtn->OnClicked.AddDynamic(this, &ThisClass::OnHistoryBtnClicked);
	MenuBtn->OnClicked.AddDynamic(this, &ThisClass::OnMenuBtnClicked);
	BackBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackBtnClicked);
	QuitBtn->OnClicked.AddDynamic(this, &ThisClass::OnQuitBtnClicked);
}

void UWidget_Menu::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateButtonVisibility();
}

void UWidget_Menu::OnStartBtnClicked()
{
	if (!IsValid(mDialogueWidgetClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("UWidget_Menu::mDialogueWidgetClass 클래스 없음"));
		return;
	}
	if (!IsValid(mDialogueWidget))
	{
		mDialogueWidget =
			CreateWidget<UWidget_Dialogue>(GetOwningPlayer(), mDialogueWidgetClass);
	}
	if (!IsValid(mDialogueWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("UWidget_Menu::mDialogueWidget 캐스팅 실패"));
		return;
	}
	mDialogueWidget->Init(this,UBFL_VN::mDialogue,mParicipants);
	mDialogueWidget->AddToViewport(0);
	UpdateButtonVisibility();
}

void UWidget_Menu::OnHistoryBtnClicked()
{
	if (!IsValid(mDialogueWidget))
	{
		return;
	}
	if(!mDialogueWidget->IsInViewport())
	{
		OnBackBtnClicked();
		return;
	}
	mDialogueWidget->RemoveFromParent();
	MenuWS->SetActiveWidgetIndex(1);
	UpdateButtonVisibility();
}

void UWidget_Menu::OnMenuBtnClicked()
{
	MenuWS->SetActiveWidgetIndex(0);
	mDialogueWidget->RemoveFromParent();
	mDialogueWidget->Init(this,nullptr, mParicipants);
	UpdateButtonVisibility();
}

void UWidget_Menu::OnBackBtnClicked()
{
	mDialogueWidget->AddToViewport(0);
	MenuWS->SetActiveWidgetIndex(0);
	UpdateButtonVisibility();
}

void UWidget_Menu::OnQuitBtnClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(),EQuitPreference::Quit,false);
}

void UWidget_Menu::UpdateButtonVisibility()
{
	if(IsInGame())
	{
		StartBtn->SetVisibility(ESlateVisibility::Collapsed);
		SaveBtn->SetVisibility(ESlateVisibility::Visible);
		CodexBtn->SetVisibility(ESlateVisibility::Visible);
		HistoryBtn->SetVisibility(ESlateVisibility::Visible);
		MenuBtn->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		StartBtn->SetVisibility(ESlateVisibility::Visible);
		SaveBtn->SetVisibility(ESlateVisibility::Collapsed);
		CodexBtn->SetVisibility(ESlateVisibility::Collapsed);
		HistoryBtn->SetVisibility(ESlateVisibility::Collapsed);
		MenuBtn->SetVisibility(ESlateVisibility::Collapsed);
	}
	if(MenuWS->ActiveWidgetIndex!=0)
	{
		BackBtn->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BackBtn->SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool UWidget_Menu::IsInGame()
{
	if(!IsValid(mDialogueWidget))
	{
		return false;
	}
	return IsValid(mDialogueWidget->GetDialogueContext());
}

void UWidget_Menu::AddEntry(FText Name, FText EntryText)
{
	History->AddEntry(Name, EntryText);
}

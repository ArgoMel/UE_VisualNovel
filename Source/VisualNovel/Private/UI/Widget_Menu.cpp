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

}

void UWidget_Menu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	StartBtn->OnClicked.AddDynamic(this, &ThisClass::OnStartBtnClicked);
	SaveBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackBtnClicked);
	LoadBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackBtnClicked);
	CodexBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackBtnClicked);
	HistoryBtn->OnClicked.AddDynamic(this, &ThisClass::OnHistoryBtnClicked);
	MenuBtn->OnClicked.AddDynamic(this, &ThisClass::OnMenuBtnClicked);
	OptionBtn->OnClicked.AddDynamic(this, &ThisClass::OnOptionBtnClicked);
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
	mDialogueWidget->StartDialogue(UBFL_VN::mDialogue);
	mDialogueWidget->AddToViewport(0);
	UpdateButtonVisibility();
}

void UWidget_Menu::OnHistoryBtnClicked()
{
	if(MenuWS->GetActiveWidgetIndex() == 1)
	{
		OnBackBtnClicked();
		return;
	}
	if (IsValid(mDialogueWidget))
	{
		mDialogueWidget->RemoveFromParent();
	}
	MenuWS->SetActiveWidgetIndex(1);
	UpdateButtonVisibility();
}

void UWidget_Menu::OnMenuBtnClicked()
{
	MenuWS->SetActiveWidgetIndex(0);
	mDialogueWidget->RemoveFromParent();
	mDialogueWidget->StartDialogue(nullptr);
	UpdateButtonVisibility();
}

void UWidget_Menu::OnOptionBtnClicked()
{
	if(MenuWS->GetActiveWidgetIndex()==2)
	{
		OnBackBtnClicked();
		return;
	}
	MenuWS->SetActiveWidgetIndex(2);
	if(IsValid(mDialogueWidget))
	{
		mDialogueWidget->RemoveFromParent();
	}
	UpdateButtonVisibility();
}

void UWidget_Menu::OnBackBtnClicked()
{
	if (IsInGame())
	{
		mDialogueWidget->AddToViewport(0);
	}
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
	if(MenuWS->GetActiveWidgetIndex()!=0)
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

void UWidget_Menu::Init(UWidget_Dialogue* DialogueWidget)
{
	mDialogueWidget = DialogueWidget;
	mDialogueWidget->Init(this);
}

void UWidget_Menu::AddEntry(FText Name, FText EntryText)
{
	History->AddEntry(Name, EntryText);
}

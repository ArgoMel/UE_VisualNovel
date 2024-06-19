#include "UI/Widget_CodexBtn.h"
#include "UI/Widget_Codex.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

UWidget_CodexBtn::UWidget_CodexBtn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UWidget_CodexBtn::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	CodexBtn->OnClicked.AddDynamic(this,&ThisClass::OnCodexBtnClicked);
}

void UWidget_CodexBtn::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_CodexBtn::OnCodexBtnClicked()
{
	mCodexWidget->mRecentCodexBtn = this;
	mCodexWidget->ShowCodexDetail(mCodexEntry);
}

void UWidget_CodexBtn::Init(UWidget_Codex* Codex, UDlgDialogue* Dialogue)
{
	mCodexWidget = Codex;
	mCodexEntry = Dialogue;
}

void UWidget_CodexBtn::SetText(FText Text)
{
	CodexText->SetText(Text);
}
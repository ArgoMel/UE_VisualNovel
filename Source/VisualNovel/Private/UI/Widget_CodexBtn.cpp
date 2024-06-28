#include "UI/Widget_CodexBtn.h"
#include "UI/Widget_Codex.h"
#include "UI/Widget_Dialogue.h"
#include <BFL/BFL_VN.h>
#include "DlgSystem/DlgDialogue.h"
#include <DlgSystem/DlgManager.h>
#include "DlgSystem/DlgContext.h"
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

void UWidget_CodexBtn::UpdateCodexDetails()
{
	TArray<UObject*> participants;
	mDialogueWidget->GetParticipants(mCodexEntry, participants);
	UDlgContext* context =UDlgManager::StartDialogue(mCodexEntry, participants);
	CodexText->SetText(context->GetActiveNodeText());
	mCodexDetails.Empty();
	AddToCodexDetail(context);
}

void UWidget_CodexBtn::AddToCodexDetail(UDlgContext* CodexContext)
{
	if (!CodexContext->ChooseOption(0))
	{
		return;
	}
	mCodexDetails.Add(UBFL_VN::ToTargetText(CodexContext->GetActiveNodeText()));
	AddToCodexDetail(CodexContext);
}

void UWidget_CodexBtn::OnCodexBtnClicked()
{
	mCodexWidget->ShowCodexDetail(CodexText->GetText(), mCodexDetails, this);
}

void UWidget_CodexBtn::Init(UWidget_Codex* Codex, UWidget_Dialogue* Dialogue, 
	UDlgDialogue* CodexEntry)
{
	mCodexWidget = Codex;
	mDialogueWidget = Dialogue;
	mCodexEntry = CodexEntry;
}

FText UWidget_CodexBtn::GetCodexText()
{
	return CodexText->GetText();
}

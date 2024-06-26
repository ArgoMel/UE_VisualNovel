#include "GameMode/GM_VN.h"
#include "PlayerController/PC_VN.h"
#include "GameInstance/GI_VN.h"
#include "UI/Widget_Menu.h"

AGM_VN::AGM_VN()
{
	PlayerControllerClass = APC_VN::StaticClass();
	DefaultPawnClass = nullptr;
}

void AGM_VN::BeginPlay()
{
	Super::BeginPlay();
	UGI_VN* gameInstance=Cast<UGI_VN>(GetGameInstance());
	if(!IsValid(gameInstance))
	{
		return;
	}
	gameInstance->CreateMenu();
	if(gameInstance->GetMenuWidget()->IsInGame())
	{
		gameInstance->ResumeDialogue();
	}
	else
	{
		gameInstance->ShowMenu();
	}
}

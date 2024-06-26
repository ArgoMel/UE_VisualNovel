#include "GameMode/GM_VN.h"
#include "PlayerController/PC_VN.h"
#include "GameInstance/GI_VN.h"
#include "UI/Widget_Menu.h"
#include "Actor/VNSceneCapture2D.h"
#include <Kismet/GameplayStatics.h>

AGM_VN::AGM_VN()
{
	PlayerControllerClass = APC_VN::StaticClass();
	DefaultPawnClass = nullptr;
}

void AGM_VN::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AVNSceneCapture2D::StaticClass(), actors);
	for(auto& actor:actors)
	{
		AVNSceneCapture2D* capture=Cast<AVNSceneCapture2D>(actor);
		mVNSceneCaptures.Add(capture->mBGName, capture);
	}

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

UMaterialInstance* AGM_VN::GetSceneCaptureMatByName(FName TexName, FName OldName)
{
	if (mVNSceneCaptures.Contains(OldName))
	{
		mVNSceneCaptures.FindRef(OldName)->ToggleCaptureEveryFrame();
	}
	if(!mVNSceneCaptures.Contains(TexName))
	{
		return nullptr;
	}
	AVNSceneCapture2D* capture= mVNSceneCaptures.FindRef(TexName);
	capture->ToggleCaptureEveryFrame();
	return capture->GetSceneCaptureMI();
}

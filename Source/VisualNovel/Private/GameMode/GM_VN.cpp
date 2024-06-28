#include "GameMode/GM_VN.h"
#include "PlayerController/PC_VN.h"
#include "GameInstance/GI_VN.h"
#include "UI/Widget_Menu.h"
#include "Actor/VNSceneCapture2D.h"
#include "../VisualNovel.h"
#include "Components/AudioComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Engine/AssetManager.h>

AGM_VN::AGM_VN()
{
	PlayerControllerClass = APC_VN::StaticClass();
	DefaultPawnClass = nullptr;

	mBGMComp = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMComp"));
	SetRootComponent(mBGMComp);
	mBGMComp->bAutoActivate = false;

	mVoiceComp = CreateDefaultSubobject<UAudioComponent>(TEXT("VoiceComp"));
	mVoiceComp->SetupAttachment(mBGMComp);
	mVoiceComp->bAutoActivate = false;
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

	mGameInstance =Cast<UGI_VN>(GetGameInstance());
	mGameInstance->OnAssetLoadComplete.AddUniqueDynamic(this, &ThisClass::OnAssetLoadComplete);
	mGameInstance->CreateUI();
	if(mGameInstance->IsAllAssetLoading())
	{
		OnAssetLoadComplete();
	}
	else
	{
		mGameInstance->ShowLoading(true);
	}
}

void AGM_VN::OnAssetLoadComplete()
{
	if (!mGameInstance->IsAllAssetLoading())
	{
		return;
	}
	mGameInstance->ShowLoading(false);
	if (mGameInstance->GetMenuWidget()->IsInGame())
	{
		mGameInstance->ResumeDialogue();
	}
	else
	{
		mGameInstance->ShowMenu();
		SetBGMByName(VN_START_BGM);
	}
}

UMaterialInstance* AGM_VN::GetSceneCaptureMatByName(FName TexName,
	FName OldName)
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

void AGM_VN::SetBGMByName(FName BGMName)
{
	UAssetManager& manager = UAssetManager::Get();
	FPrimaryAssetId asset = FPrimaryAssetId(PRIMARY_ASSET_TYPE_MUSIC, BGMName);
	USoundBase* sound = Cast<USoundBase>(manager.GetPrimaryAssetObject(asset));
	mBGMComp->SetSound(sound);
	mBGMComp->Play();
}

void AGM_VN::SetVoice(USoundBase* Voice)
{
	mVoiceComp->Stop();
	mVoiceComp->SetSound(Voice);
	mVoiceComp->Play();
}

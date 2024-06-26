#include "Actor/VNSceneCapture2D.h"
#include "../VisualNovel.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Camera/CameraComponent.h"
#include <Kismet/KismetRenderingLibrary.h>
#include <Kismet/KismetMaterialLibrary.h>

AVNSceneCapture2D::AVNSceneCapture2D()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

	mSceneCaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComp"));
	mSceneCaptureComp->SetupAttachment(RootComponent);
	mSceneCaptureComp->bCaptureEveryFrame = false;

	mCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	mCameraComp->SetupAttachment(mSceneCaptureComp);

	GetObjectAsset(mSceneCaptureMat, UMaterialInterface, "/Game/VN/Material/M_SceneCapture.M_SceneCapture");
}

void AVNSceneCapture2D::BeginPlay()
{
	Super::BeginPlay();
	mSceneCaptureComp->TextureTarget =
		UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(),1920,1080);
	UMaterialInstanceDynamic* mat=
		UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), mSceneCaptureMat);
	mat->SetTextureParameterValue(MAT_TEX_PARAMETER_VIEWTARGET, mSceneCaptureComp->TextureTarget);
	mSceneCaptureMI = mat;
}

void AVNSceneCapture2D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVNSceneCapture2D::ToggleCaptureEveryFrame()
{
	if(mSceneCaptureComp->bCaptureEveryFrame)
	{
		mSceneCaptureComp->bCaptureEveryFrame = false;
	}
	else
	{
		mSceneCaptureComp->bCaptureEveryFrame = true;
	}
}


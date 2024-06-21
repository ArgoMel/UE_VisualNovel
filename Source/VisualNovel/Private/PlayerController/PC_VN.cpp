#include "PlayerController/PC_VN.h"
#include "GameInstance/GI_VN.h"

APC_VN::APC_VN()
{
	bShowMouseCursor = true;
}

void APC_VN::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(input))
	{
		return;
	}
	EChessInputAction::BindInput_StartOnly(input, mVNInputAsset->mToggleGameAndMenu, this, &ThisClass::ToggleGameAndMenu);
}

void APC_VN::BeginPlay()
{
	Super::BeginPlay();
	mGameInstance = Cast<UGI_VN>(GetGameInstance());
}

void APC_VN::ToggleGameAndMenu()
{
	if(!IsValid(mGameInstance))
	{
		return;
	}
	mGameInstance->ToggleGameAndMenu();
	OnMouseRightClicked.Broadcast();
}

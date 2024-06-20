#include "PlayerController/PC_Input.h"
#include "../VisualNovel.h"
#include "EnhancedInputSubsystems.h"

APC_Input::APC_Input()
{
	GetObjectAsset(mVNInputAsset, UVNInput, "/Game/VN/Input/DA_VNInput.DA_VNInput");
}

void APC_Input::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputLocalPlayerSubsystem* inputSubSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(inputSubSystem))
	{
		return;
	}
	inputSubSystem->ClearAllMappings();
	SetInput(mVNInputAsset->mDefaultMappingContext);
}

void APC_Input::AddInputMapping(const UInputMappingContext* InputMapping, 
	const int32 MappingPriority) const
{
	UEnhancedInputLocalPlayerSubsystem* inputSubSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(inputSubSystem))
	{
		return;
	}
	ensure(InputMapping);
	if (!inputSubSystem->HasMappingContext(InputMapping))
	{
		inputSubSystem->AddMappingContext(InputMapping, MappingPriority);
	}
}

void APC_Input::RemoveInputMapping(const UInputMappingContext* 
	InputMapping) const
{
	UEnhancedInputLocalPlayerSubsystem* inputSubSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(inputSubSystem))
	{
		return;
	}
	ensure(InputMapping);
	inputSubSystem->RemoveMappingContext(InputMapping);
}

void APC_Input::SetInput(const UInputMappingContext* InputMapping, 
	const bool Enabled) const
{
	ensureMsgf(mVNInputAsset, TEXT("%s is NULL!"), *mVNInputAsset->GetFName().ToString());
	ensure(mVNInputAsset->mDefaultMappingContext);
	if (Enabled)
	{
		AddInputMapping(mVNInputAsset->mDefaultMappingContext, mVNInputAsset->mDefaultMapPriority);
	}
	else
	{
		RemoveInputMapping(mVNInputAsset->mDefaultMappingContext);
	}
}

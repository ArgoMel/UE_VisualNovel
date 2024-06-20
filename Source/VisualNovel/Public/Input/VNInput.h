#pragma once
#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Engine/DataAsset.h"
#include "VNInput.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS(BlueprintType)
class VISUALNOVEL_API UVNInput : public UDataAsset
{
	GENERATED_BODY()
public:
	UVNInput();

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	TObjectPtr<UInputMappingContext> mDefaultMappingContext;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	TObjectPtr<UInputAction> mToggleGameAndMenu;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	int32 mDefaultMapPriority;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Shift")
	TObjectPtr<UInputMappingContext> mShift;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Shift")
	TObjectPtr<UInputAction> mShiftSelect;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Shift")
	int32 mShiftMapPriority;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Alt")
	TObjectPtr<UInputMappingContext> mAlt;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Alt")
	TObjectPtr<UInputAction> mAltSelect;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Alt")
	int32 mAltMapPriority;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ctrl")
	TObjectPtr<UInputMappingContext> mCtrl;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ctrl")
	TObjectPtr<UInputAction> mCtrlSelect;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ctrl")
	int32 mCtrlMapPriority;
};

namespace EChessInputAction
{
	template<class T, class FuncType>
	void BindInput_StartOnly(UEnhancedInputComponent* Input, const UInputAction* Action, T* Obj,
		FuncType StartFunc)
	{
		if(!Action)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInputAction:%s 없음"), *Action->GetName());
			return;
		}
		if (StartFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Started, Obj, StartFunc);
		}
	}
	template<class T, class FuncType>
	void BindInput_TriggerOnly(UEnhancedInputComponent* Input, const UInputAction* Action, T* Obj,
		FuncType TriggerFunc)
	{
		if (!Action)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInputAction:%s 없음"), *Action->GetName());
			return;
		}
		if (TriggerFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Triggered, Obj, TriggerFunc);
		}
	}
	template<class T, class FuncType>
	void BindInput_StartTriggerComplete(UEnhancedInputComponent* Input, const UInputAction* Action, T* Obj,
		FuncType StartFunc, FuncType TriggerFunc, FuncType CompleteFunc)
	{
		if (!Action)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInputAction:%s 없음"), *Action->GetName());
			return;
		}
		if (StartFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Started, Obj, StartFunc);
		}
		if (TriggerFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Triggered, Obj, TriggerFunc);
		}
		if (CompleteFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Completed, Obj, CompleteFunc);
		}
	}
	template<class T, class FuncType>
	void BindInput_TriggerComplete(UEnhancedInputComponent* Input, const UInputAction* Action, T* Obj,
		FuncType StartFunc, FuncType CompleteFunc)
	{
		if (!Action)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInputAction:%s 없음"), *Action->GetName());
			return;
		}
		if (StartFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Started, Obj, StartFunc);
		}
		if (CompleteFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Completed, Obj, CompleteFunc);
		}
	}
	template<class T, class FuncType>
	void BindInput_StartTriggerCompleteCancel(UEnhancedInputComponent* Input, const UInputAction* Action, T* Obj,
		FuncType StartFunc, FuncType TriggerFunc, FuncType CompleteFunc, FuncType CancelFunc)
	{
		if (!Action)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInputAction:%s 없음"), *Action->GetName());
			return;
		}
		if (StartFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Started, Obj, StartFunc);
		}
		if (TriggerFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Triggered, Obj, TriggerFunc);
		}
		if (CompleteFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Completed, Obj, CompleteFunc);
		}
		if (CancelFunc)
		{
			Input->BindAction(Action, ETriggerEvent::Canceled, Obj, CancelFunc);
		}
	}
}
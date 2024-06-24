#pragma once
#include "CoreMinimal.h"

#pragma region LoadAsset
/*    Object Asset을 받습니다.
*    RETURN_POINTER : 받을 포인터
*    CLASS_TYPE : 받을 오브젝트의 클래스
*    PATH : 경로 (TEXT를 붙이지 않습니다.)*/
#define GetObjectAsset(RETURN_POINTER, CLASS_TYPE, PATH)\
static ConstructorHelpers::FObjectFinder<CLASS_TYPE> __##RETURN_POINTER(TEXT(PATH));\
if (__##RETURN_POINTER.Succeeded()) \
{\
    RETURN_POINTER = __##RETURN_POINTER.Object;\
}\
else \
{\
	UE_LOG(LogTemp,Warning,TEXT("Failed GetObjectAsset : %d"),TEXT(PATH));\
	RETURN_POINTER = nullptr;\
}

#define AddObjectAsset(RETURN_POINTER, CLASS_TYPE, PATH)\
{\
	static ConstructorHelpers::FObjectFinder<CLASS_TYPE> __##RETURN_POINTER(TEXT(PATH));\
	if (__##RETURN_POINTER.Succeeded()) \
	{\
	    RETURN_POINTER.Add(__##RETURN_POINTER.Object);\
	}\
	else \
	{\
		UE_LOG(LogTemp,Warning,TEXT("Failed GetObjectAsset : %d"),TEXT(PATH));\
		RETURN_POINTER.Add(nullptr);\
	}\
}

/*    Class Asset을 받습니다.
*    RETURN_POINTER : 받을 포인터
*    CLASS_TYPE : 받을 클래스 타입
*    PATH : 경로 (TEXT를 붙이지 않습니다.)*/
#define GetClassAsset(RETURN_POINTER, CLASS_TYPE, PATH)\
static ConstructorHelpers::FClassFinder<CLASS_TYPE> __##RETURN_POINTER(TEXT(PATH));\
if (__##RETURN_POINTER.Succeeded()) \
{\
    RETURN_POINTER = __##RETURN_POINTER.Class;\
}\
else \
{\
	UE_LOG(LogTemp,Warning,TEXT("Failed GetClassAsset : %d"),TEXT(PATH));\
	RETURN_POINTER = nullptr;\
}
#pragma endregion

#define PRIMARY_ASSET_TYPE_CODEX FName(TEXT("Codex"))
#define PRIMARY_ASSET_TYPE_GALLERY FName(TEXT("GalleryImg"))
#define PRIMARY_ASSET_TYPE_SCRIPT FName(TEXT("Script"))

#define SLOTNAME_PERSISTANTDATA TEXT("PersistantData")
#define SLOTNAME_QUICK_SAVE TEXT("QuickSave")

#define PARTICIPANTNAME_WIDGET TEXT("Widget")
#define PARTICIPANTNAME_GAMEINSTANCE TEXT("GameInstance")
#define PARTICIPANTNAME_UNKNOWN TEXT("???")
#define PARTICIPANTNAME_DEFAULT TEXT("Argo")

#define VALUENAME_NOTIFY TEXT("Notify")
#define VALUENAME_CHANGE_BG_SLIDE TEXT("ChangeBG_Slide")
#define VALUENAME_CHANGE_BG_FADE TEXT("ChangeBG_Fade")
#define VALUENAME_CHANGE_BG_DESOLVE TEXT("ChangeBG_Desolve")
#define VALUENAME_CHANGE_BG_WIPE TEXT("ChangeBG_Wipe")
#define VALUENAME_CHANGE_SCRIPT TEXT("ChangeScript")

#define VN_START_BG TEXT("TX_Modern-Dormroom_001")
#define VN_START_SCRIPT TEXT("Dlg_Test")
#define VN_START_EXPRESSION TEXT("Happy")
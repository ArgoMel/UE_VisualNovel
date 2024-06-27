#include "Save/PersistantData.h"
#include "../VisualNovel.h"

UPersistantData::UPersistantData()
{
	mTextSpeed = 0.5f;
	bShowUnselectableOption = false;
	bShowPreviouslyPickedChoices = true;

	mSkipSpeed = 0.001f;
	bCancelSkipOnOptions = false;

	mAutoSpeed = 0.5f;
	bCancelAutoOnOptions = false;

	mSaveNames.Add(SLOTNAME_QUICK_SAVE);
	for(int32 i=1;i<100;++i)
	{
		mSaveNames.Add(FString::Printf(TEXT("저장 데이터 %03d"),i));
	}

	mVolumes.Init(1.f, (int32)ESoundKind::Max);
}

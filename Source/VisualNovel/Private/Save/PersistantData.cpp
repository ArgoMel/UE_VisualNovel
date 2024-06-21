#include "Save/PersistantData.h"

UPersistantData::UPersistantData()
{
	mTextSpeed = 0.5f;
	bShowUnselectableOption = false;
	bShowPreviouslyPickedChoices = true;

	mSkipSpeed = 0.001f;
	bCancelSkipOnOptions = false;

	mAutoSpeed = 0.5f;
	bCancelAutoOnOptions = false;
}

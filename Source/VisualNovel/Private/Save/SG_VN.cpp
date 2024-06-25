#include "Save/SG_VN.h"
#include "../VisualNovel.h"

USG_VN::USG_VN()
{
	mCurDialogueName = VN_START_SCRIPT;
	mCurBG = VN_START_BG;
	mActiveNodeIndex = 0;
	bAskForPlayerName = false;

	mSaveTime = FDateTime::Now();
	mPlayerName = PARTICIPANTNAME_DEFAULT;
}

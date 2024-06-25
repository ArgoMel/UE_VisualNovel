#include "BFL/BFL_VN.h"
#include "Save/PersistantData.h"
#include "../VisualNovel.h"
#include "DlgSystem/DlgDialogue.h"
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include <Kismet/GameplayStatics.h>

TObjectPtr<UStringTable> UBFL_VN::mKeywordData;
TObjectPtr<UDataTable> UBFL_VN::mParticipantData;
FString UBFL_VN::mRecentSlotName;

UBFL_VN::UBFL_VN()
{
	GetObjectAsset(mKeywordData, UStringTable, "/Game/VN/Data/ST_Keyword.ST_Keyword");
	GetObjectAsset(mParticipantData, UDataTable, "/Game/VN/Data/DT_ParticipantData.DT_ParticipantData");
}

bool UBFL_VN::GetKeyword(FString InText, FString& OutText)
{
	return mKeywordData->GetStringTable()->GetSourceString(InText.ToUpper(), OutText);
}

FParticipantData UBFL_VN::GetParticipantData(EParticipantName Name)
{
	return *mParticipantData->FindRow<FParticipantData>(*UEnum::GetDisplayValueAsText(Name).ToString(), TEXT(""), false);
}

FString UBFL_VN::ToTargetString(FText InText, bool AddQuotes, 
	bool UseSmallStyle)
{
	FString processString = InText.ToString();
	if (!processString.StartsWith(TEXT("<")))
	{
		processString = TEXT("<Normal>") + processString;
	}

	if (AddQuotes)
	{
		int32 index = INDEX_NONE;
		processString.FindChar('>', index);
		processString.InsertAt(index + 1, TEXT("\""));
		processString += TEXT("\"");
	}

	TArray<FString> words;
	processString.ParseIntoArray(words, TEXT(" "), true);
	processString.Empty();
	FString lastTextStyle;
	for (FString& curStr : words)
	{
		int32 index = INDEX_NONE;
		curStr.FindChar('>', index);
		if (index != INDEX_NONE)
		{
			lastTextStyle = curStr.Mid(0, index + 1);
		}
		FString noSymbolStr = RemoveSymbolText(curStr);
		FString keyword;
		if (GetKeyword(noSymbolStr, keyword))
		{
			FString rSymbolStr;
			FString lSymbolStr;
			curStr.Split(noSymbolStr, &rSymbolStr, &lSymbolStr);

			processString += rSymbolStr;
			processString += keyword;
			processString += noSymbolStr;
			processString += lastTextStyle;
			processString += lSymbolStr;
		}
		else
		{
			processString += curStr;
		}
		processString += TEXT(" ");
	}

	if(UseSmallStyle)
	{
		processString.ReplaceInline(TEXT(">"), TEXT("Small>"));
	}

	processString.ReplaceInline(TEXT("<"), TEXT("</><"));
	processString = processString.RightChop(3);
	processString = processString + TEXT("</>");
	return processString;
}

FText UBFL_VN::ToTargetText(FText InText, bool AddQuotes, 
	bool UseSmallStyle)
{
	return FText::FromString(ToTargetString(InText, AddQuotes, UseSmallStyle));
}

FString UBFL_VN::RemoveSymbolText(FString InText)
{
	FString tempStr = InText;
	InText.Split(TEXT(">"), nullptr, &tempStr);
	tempStr=tempStr.Replace(TEXT("."), TEXT(""));
	tempStr=tempStr.Replace(TEXT("?"), TEXT(""));
	tempStr=tempStr.Replace(TEXT("!"), TEXT(""));
	tempStr=tempStr.Replace(TEXT(","), TEXT(""));
	tempStr=tempStr.Replace(TEXT("\""), TEXT(""));
	tempStr=tempStr.Replace(TEXT("'"), TEXT(""));
	return tempStr;
}

void UBFL_VN::TakeScreenshotOfUI(FString Name)
{
	mRecentSlotName = Name;
	FScreenshotRequest::RequestScreenshot(Name,true,false);
}

void UBFL_VN::GetAllSaveGameSlotNames(TArray<FString>& Names)
{
	if (!UGameplayStatics::DoesSaveGameExist(SLOTNAME_PERSISTANTDATA, 0))
	{
		return;
	}
	UPersistantData* persistantData = Cast<UPersistantData>(
		UGameplayStatics::LoadGameFromSlot(SLOTNAME_PERSISTANTDATA, 0));
	Names = persistantData->mSaveNames;
}
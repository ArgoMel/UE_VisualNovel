#include "BFL/BFL_VN.h"
#include "Save/PersistantData.h"
#include "../VisualNovel.h"
#include "DlgSystem/DlgDialogue.h"
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include <Kismet/GameplayStatics.h>
#include "AudioDevice.h"
#include <Engine/AssetManager.h>

TObjectPtr<UStringTable> UBFL_VN::mKeywordData;
TObjectPtr<UStringTable> UBFL_VN::mStyleData;
TObjectPtr<UDataTable> UBFL_VN::mParticipantData;
TObjectPtr<USoundMix> UBFL_VN::mSoundMix;
FString UBFL_VN::mRecentSlotName;

UBFL_VN::UBFL_VN()
{
	GetObjectAsset(mKeywordData, UStringTable, "/Game/VN/Data/ST_Keyword.ST_Keyword");
	GetObjectAsset(mStyleData, UStringTable, "/Game/VN/Data/ST_Style.ST_Style");
	GetObjectAsset(mParticipantData, UDataTable, "/Game/VN/Data/DT_ParticipantData.DT_ParticipantData");
	GetObjectAsset(mSoundMix, USoundMix, "/Game/VN/Sound/SCM_VN.SCM_VN");
}

bool UBFL_VN::GetKeyword(FString InText, FString& OutText)
{
	return mKeywordData->GetStringTable()->GetSourceString(InText.ToUpper(), OutText);
}

bool UBFL_VN::GetStyleword(FString InText, FString& OutText)
{
	return mStyleData->GetStringTable()->GetSourceString(InText.ToUpper(), OutText);
}

FParticipantData UBFL_VN::GetParticipantData(EParticipantName Name)
{
	return *mParticipantData->FindRow<FParticipantData>(*UEnum::GetDisplayValueAsText(Name).ToString(), TEXT(""), false);
}

FString UBFL_VN::ToTargetString(FText InText, bool AddQuotes, 
	int32 TextSize)
{
	FString processString = InText.ToString();
	if (!processString.StartsWith(TEXT("<")))
	{
		processString = TEXT("<>") + processString;
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
	for (FString& word : words)
	{
		int32 index = INDEX_NONE;
		word.FindChar('>', index);
		if (index != INDEX_NONE)
		{
			lastTextStyle = word.Mid(0, index + 1);
		}
		FString noSymbolStr = RemoveSymbolText(word);
		FString keyword;
		FString styleword;
		if (GetKeyword(noSymbolStr, keyword))
		{
			TArray<FString> stylewords;
			keyword.ParseIntoArray(stylewords, TEXT(","), false);
			for (FString& tempword : stylewords)
			{
				FString tempStr;
				GetStyleword(tempword, tempStr);
				styleword += tempStr;
			}
			FString lSymbolStr;
			FString rSymbolStr;
			word.Split(noSymbolStr, &lSymbolStr, &rSymbolStr);

			processString += lSymbolStr;
			processString += TEXT("<");
			processString += styleword;
			processString += TEXT(">");
			processString += noSymbolStr;
			processString += lastTextStyle;
			processString += rSymbolStr;
		}
		else
		{
			processString += word;
		}
		processString += TEXT(" ");
	}
	processString = processString.LeftChop(1);

	const FString fontSize = FString::Printf(TEXT(" size=\"%d\">"), TextSize);
	processString.ReplaceInline(TEXT(">"), *fontSize);

	processString.ReplaceInline(TEXT("<"), TEXT("</><span"));
	processString = processString.RightChop(3);
	processString += TEXT("</>");
	return processString;
}

FText UBFL_VN::ToTargetText(FText InText, bool AddQuotes, 
	int32 TextSize)
{
	return FText::FromString(ToTargetString(InText, AddQuotes, TextSize));
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

void UBFL_VN::SetVolume(const UObject* WorldContextObject,float Value, 
	ESoundKind Sound)
{
	FString name = TEXT("SC_") + UEnum::GetDisplayValueAsText(Sound).ToString();
	UAssetManager& manager = UAssetManager::Get();
	FPrimaryAssetId asset = 
		FPrimaryAssetId(PRIMARY_ASSET_TYPE_SOUNDCLASS, FName(name));
	USoundClass* soundClass=
		Cast<USoundClass>(manager.GetPrimaryAssetObject(asset));
	UGameplayStatics::SetSoundMixClassOverride(WorldContextObject, 
		mSoundMix, soundClass, Value);
}

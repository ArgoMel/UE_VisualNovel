#include "BFL/BFL_VN.h"
#include "../VisualNovel.h"
#include "DlgSystem/DlgDialogue.h"
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"

TObjectPtr<UStringTable> UBFL_VN::mKeywordData;
TObjectPtr<UDataTable> UBFL_VN::mParticipantData;
TObjectPtr<UDataTable> UBFL_VN::mBGImgData;
TObjectPtr<UDlgDialogue> UBFL_VN::mDialogue;

UBFL_VN::UBFL_VN()
{
	GetObjectAsset(mKeywordData, UStringTable, "/Game/VN/Data/ST_Keyword.ST_Keyword");
	GetObjectAsset(mParticipantData, UDataTable, "/Game/VN/Data/DT_ParticipantData.DT_ParticipantData");
	GetObjectAsset(mBGImgData, UDataTable, "/Game/VN/Data/DT_BGImgData.DT_BGImgData");
	GetObjectAsset(mDialogue, UDlgDialogue, "/Game/VN/Dialogue/Dlg_Test.Dlg_Test");
}

bool UBFL_VN::GetKeyword(FString InText, FString& OutText)
{
	return mKeywordData->GetStringTable()->GetSourceString(InText.ToUpper(), OutText);
}

FParticipantData UBFL_VN::GetParticipantData(EParticipantName Name)
{
	return *mParticipantData->FindRow<FParticipantData>(*UEnum::GetDisplayValueAsText(Name).ToString(), TEXT(""), false);
}

FBGImgData UBFL_VN::GetBGImgData(FName Name)
{
	return *mBGImgData->FindRow<FBGImgData>(Name, TEXT(""), false);
}

FString UBFL_VN::ToTargetString(FText InText, bool AddQuotes)
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

	processString.ReplaceInline(TEXT("<"), TEXT("</><"));
	processString = processString.RightChop(3);
	processString = processString + TEXT("</>");
	return processString;
}

FText UBFL_VN::ToTargetText(FText InText, bool AddQuotes)
{
	return FText::FromString(ToTargetString(InText, AddQuotes));
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

float UBFL_VN::FakeLerp(float CurValue, float TargetValue, float DelataTime, 
	float Speed)
{

	if (FMath::IsNearlyEqual(CurValue, TargetValue))
	{
		return TargetValue;
	}
	if (CurValue < TargetValue)
	{
		return FMath::Min(Speed * DelataTime + CurValue, TargetValue);
	}
	return FMath::Max(-Speed * DelataTime + CurValue, TargetValue);
}

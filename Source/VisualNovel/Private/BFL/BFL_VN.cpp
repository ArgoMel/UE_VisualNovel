#include "BFL/BFL_VN.h"
#include "../VisualNovel.h"
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"

TObjectPtr<UStringTable> UBFL_VN::mKeywordData;

UBFL_VN::UBFL_VN()
{
	GetObjectAsset(mKeywordData, UStringTable, "/Game/VN/ST_Keyword.ST_Keyword");
}

bool UBFL_VN::GetKeyword(FString InText, FString& OutText)
{
	return mKeywordData->GetStringTable()->GetSourceString(InText.ToUpper(), OutText);
}

FString UBFL_VN::ToTargetText(FText InText, bool AddQuotes)
{
	FString processString = InText.ToString();
	if(!processString.StartsWith(TEXT("<")))
	{
		processString = TEXT("<Normal>") + processString;
	}

	if(AddQuotes)
	{
		int32 index = INDEX_NONE;
		processString.FindChar('>', index);
		processString.InsertAt(index+1, TEXT("\""));
		processString+= TEXT("\"");
	}

	TArray<FString> words;
	processString.ParseIntoArray(words, TEXT(" "),true);
	processString.Empty();
	FString lastTextStyle;
	for (FString& curStr : words)
	{
		int32 index = INDEX_NONE;
		curStr.FindChar('>', index);
		if(index!=INDEX_NONE)
		{			
			lastTextStyle = curStr.Mid(0, index+1);
		}
		FString noSymbolStr = RemoveSymbolText(curStr);
		FString keyword;
		if(GetKeyword(noSymbolStr, keyword))
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
	processString=processString.RightChop(3);
	processString= processString + TEXT("</>");
	return processString;
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

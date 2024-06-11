#include "BFL/BFL_VN.h"

FString UBFL_VN::ToTargetText(FText InText, bool AddQuotes)
{
	FString processString = InText.ToString();
	if(!processString.StartsWith(TEXT("<")))
	{
		processString = TEXT("<Normal>") + processString;
	}

	if(AddQuotes)
	{
		FString leftStr;
		FString rightStr;
		processString.Split(TEXT(">"), &leftStr, &rightStr);
		processString= leftStr+ TEXT(">\"") + processString + TEXT("\"");
	}

	processString.ReplaceInline(TEXT("<"), TEXT("</><"));
	processString=processString.RightChop(3);
	processString= processString + TEXT("</>");
	return processString;
}

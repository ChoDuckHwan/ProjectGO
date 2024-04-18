// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Character/Abilities/Data/DA_AttributeInfo.h"

FGOAttributeInfo UDA_AttributeInfo::FindAttributeInfoFroTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for(const auto& AssetTagInfo : AttributeInfos)
	{
		if(AssetTagInfo.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return AssetTagInfo;
		}
	}

	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't Find Info for AttributeTag [%s] on AttributeInfo [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FGOAttributeInfo();
}

const TArray<FGOAttributeInfo>& UDA_AttributeInfo::GetAttributeInfos()
{
	return AttributeInfos;
}

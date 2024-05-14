// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Character/Abilities/Data/AbilityInfo.h"

FGOAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for(const auto& AbilityInfo : AbilityInfos)
	{
		if(AbilityInfo.AbilityTag == AbilityTag)
		{
			return AbilityInfo;
		}
	}
	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityTag on AbilityTag [%s]"), *AbilityTag.ToString());
	}
	return FGOAbilityInfo();
}

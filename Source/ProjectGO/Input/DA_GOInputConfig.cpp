// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Input/DA_GOInputConfig.h"

const UInputAction* UDA_GOInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for(const auto& AbilityInputAction : AbilityInputActions)
	{
		if(AbilityInputAction.InputAction && AbilityInputAction.InputTag == InputTag)
		{
			return AbilityInputAction.InputAction;
		}
	}
	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for inputtag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}

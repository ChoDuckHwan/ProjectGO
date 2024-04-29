// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAbilitySystemGlobals.h"
#include "GOAbilityTypes.h"

FGameplayEffectContext* UGOAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FGOGameplayEffectContext();
}

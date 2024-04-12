// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Character/Abilities/GOAbilitySystemComponent.h"

void UGOAbilitySystemComponent::ReceiveDamage(UGOAbilitySystemComponent* Source, float UnmitigatedDamage,	float MitigatedDamage)
{
	ReceiveDamage(Source, UnmitigatedDamage, MitigatedDamage);
}

void UGOAbilitySystemComponent::AbilityActorInfoSet()
{
	//Called In server
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::EffectApplied);
}

void UGOAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GES, FActiveGameplayEffectHandle AGEH)
{
	FGameplayTagContainer GameplayTagContainer;
	GES.GetAllAssetTags(GameplayTagContainer);

	EffectApplied_Client(GameplayTagContainer);
}

void UGOAbilitySystemComponent::EffectApplied_Client_Implementation(const FGameplayTagContainer& GameplayTagContainer)
{
	EffectAssetTags.Broadcast(GameplayTagContainer);
}
 
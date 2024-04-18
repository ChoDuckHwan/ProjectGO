// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Character/Abilities/GOAbilitySystemComponent.h"

#include "GOGameplayAbilityBase.h"
#include "ProjectGO/GOGameplayTags.h"

void UGOAbilitySystemComponent::ReceiveDamage(UGOAbilitySystemComponent* Source, float UnmitigatedDamage,	float MitigatedDamage)
{
	ReceiveDamage(Source, UnmitigatedDamage, MitigatedDamage);
}

void UGOAbilitySystemComponent::AbilityActorInfoSet()
{
	//Delegate called In server
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::EffectApplied);

	const FGOGameplayTags& GOGameplayTags = FGOGameplayTags::Get();
	//GOGameplayTags.Attributes_Secondary_Armor.ToString();
}

void UGOAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for(const TSubclassOf<UGameplayAbility>& AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if(const UGOGameplayAbilityBase* GoAbility = Cast<UGOGameplayAbilityBase>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(GoAbility->GetAbilityInputTag());
			GiveAbility(AbilitySpec);
		}
		//GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UGOAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for(auto& AbilitySpec : GetActivatableAbilities())
	{
		AbilitySpecInputPressed(AbilitySpec);
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && !AbilitySpec.IsActive())
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UGOAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		AbilitySpecInputReleased(AbilitySpec);
	}
}

void UGOAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GES, FActiveGameplayEffectHandle AGEH)
{
	//server only
	FGameplayTagContainer GameplayTagContainer;
	GES.GetAllAssetTags(GameplayTagContainer);

	EffectApplied_Client(GameplayTagContainer);
}

void UGOAbilitySystemComponent::EffectApplied_Client_Implementation(const FGameplayTagContainer& GameplayTagContainer)
{
	EffectAssetTags.Broadcast(GameplayTagContainer);
}
 
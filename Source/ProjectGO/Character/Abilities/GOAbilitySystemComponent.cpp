// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Character/Abilities/GOAbilitySystemComponent.h"

#include "GOGameplayAbilityBase.h"
#include "Net/UnrealNetwork.h"
#include "ProjectGO/GOGameplayTags.h"

void UGOAbilitySystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UGOAbilitySystemComponent, bAbilitiesGiven);
}

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
	bAbilitiesGiven = true;
	//AbilitiesGivenDelegate.Broadcast();
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

void UGOAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActivateScopeLock(*this);
	for(const auto& GampplaySpec : GetActivatableAbilities())
	{
		if(!Delegate.ExecuteIfBound(GampplaySpec))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag UGOAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& GameplayAbilitySpec)
{
	if(GameplayAbilitySpec.Ability)
	{
		for(const auto& Tag : GameplayAbilitySpec.Ability.Get()->AbilityTags)
		{
			if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}						
	}
	return FGameplayTag();
}

FGameplayTag UGOAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& GameplayAbilitySpec)
{
	for(const auto& InputTag : GameplayAbilitySpec.DynamicAbilityTags)
	{
		if(InputTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return InputTag;
		}
	}
	return FGameplayTag();
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

void UGOAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	if(!bAbilitiesGiven)
	{
		bAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}

void UGOAbilitySystemComponent::OnRep_AbilitiesGiven()
{
	if(bAbilitiesGiven)
	{
		//Client & Server
		AbilitiesGivenDelegate.Broadcast();
	}
}
 
// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Character/Abilities/GOGameplayAbilityBase.h"

#include "AbilitySystemComponent.h"


UGOGameplayAbilityBase::UGOGameplayAbilityBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Debug.Stun")));
}

const FGameplayTag& UGOGameplayAbilityBase::GetAbilityInputTag() const
{
	return AbilityInputTag;
}

void UGOGameplayAbilityBase::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

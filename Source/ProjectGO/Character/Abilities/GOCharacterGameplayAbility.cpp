// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Character/Abilities/GOCharacterGameplayAbility.h"
#include "ProjectGO/Character/Abilities/GOAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"

UGOCharacterGameplayAbility::UGOCharacterGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Debug.Stun")));

}

void UGOCharacterGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo,	const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if(ActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

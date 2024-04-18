// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GOAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UGOAbilitySystemComponent*, Source, float, UnmitigatedDamage, float, MitigatedDamage);
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
/**
 * 
 */
UCLASS()
class PROJECTGO_API UGOAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public :
	bool AbilitiesGiven = false;
	bool StartupEffectsApplied = false;

	FReceivedDamageDelegate ReceivedDamage;

	virtual void ReceiveDamage(UGOAbilitySystemComponent* Source, float UnmitigatedDamage, float MitigatedDamage);

	void AbilityActorInfoSet();

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	FEffectAssetTags EffectAssetTags;

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	

protected:
	void EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GES, FActiveGameplayEffectHandle AGEH);

	UFUNCTION(Client, Reliable)
	void EffectApplied_Client(const FGameplayTagContainer& GameplayTagContainer);
	void EffectApplied_Client_Implementation(const FGameplayTagContainer& GameplayTagContainer);

};

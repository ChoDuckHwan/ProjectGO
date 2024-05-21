// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GOAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UGOAbilitySystemComponent*, Source, float, UnmitigatedDamage, float, MitigatedDamage);
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
/**
 * 
 */
UCLASS()
class PROJECTGO_API UGOAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public :
	UPROPERTY(ReplicatedUsing=OnRep_AbilitiesGiven)
	bool bAbilitiesGiven = false;
	bool StartupEffectsApplied = false;

	FReceivedDamageDelegate ReceivedDamage;

	virtual void ReceiveDamage(UGOAbilitySystemComponent* Source, float UnmitigatedDamage, float MitigatedDamage);

	void AbilityActorInfoSet();

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities);

	FEffectAssetTags EffectAssetTags;
	FAbilitiesGiven AbilitiesGivenDelegate;

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbility& Delegate);
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& GameplayAbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& GameplayAbilitySpec);
	
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);
	void ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag);

protected:
	void EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GES, FActiveGameplayEffectHandle AGEH);
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION(Client, Reliable)
	void EffectApplied_Client(const FGameplayTagContainer& GameplayTagContainer);
	void EffectApplied_Client_Implementation(const FGameplayTagContainer& GameplayTagContainer);

	virtual void OnRep_ActivateAbilities() override;
	UFUNCTION()
	virtual void OnRep_AbilitiesGiven();
};

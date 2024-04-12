// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GOAttributeSetBase.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}

	FGameplayEffectContextHandle GameplayEffectContextHandle;

	UPROPERTY()	TObjectPtr<UAbilitySystemComponent> SourceASC = nullptr;
	UPROPERTY()	TObjectPtr<AActor> SourceAvatarActor = nullptr;
	UPROPERTY()	TObjectPtr<AController> SourceController = nullptr;
	UPROPERTY()	TObjectPtr<ACharacter> SourceCharacter = nullptr;

	UPROPERTY()	TObjectPtr<UAbilitySystemComponent> TargetASC = nullptr;
	UPROPERTY()	TObjectPtr<AActor> TargetAvatarActor = nullptr;
	UPROPERTY()	TObjectPtr<AController> TargetController = nullptr;
	UPROPERTY()	TObjectPtr<ACharacter> TargetCharacter = nullptr;
};
/**
 * 
 */
UCLASS()
class PROJECTGO_API UGOAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	/*
	 * Primary Attributes
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes", ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UGOAttributeSetBase, Strength)

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes", ReplicatedUsing = OnRep_Intelligence)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UGOAttributeSetBase, Intelligence)

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes", ReplicatedUsing = OnRep_Resilience)
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UGOAttributeSetBase, Resilience)

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes", ReplicatedUsing = OnRep_Vigor)
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UGOAttributeSetBase, Vigor)


	/*
	 *Secondary Attributes
	 */

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UGOAttributeSetBase, Armor)

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_ArmorPenetration)
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UGOAttributeSetBase, ArmorPenetration)

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_BlockChance)
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UGOAttributeSetBase, BlockChance)

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_CriticalHitChance)
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UGOAttributeSetBase, CriticalHitChance)

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_CriticalHitDamage)
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UGOAttributeSetBase, CriticalHitDamage)

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_CriticalHitResistance)
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UGOAttributeSetBase, CriticalHitResistance)

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_HealthRegeneration)
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UGOAttributeSetBase, HealthRegeneration)

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_ManaRegeneration)
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UGOAttributeSetBase, ManaRegeneration)

	UPROPERTY(BlueprintReadOnly, Category = "Vital Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGOAttributeSetBase, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Vital Attributes", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UGOAttributeSetBase, MaxMana)

	/*
	 *Vital Attributes
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Vital Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGOAttributeSetBase, Health)
		
	UPROPERTY(BlueprintReadOnly, Category = "Vital Attributes", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UGOAttributeSetBase, Mana)	

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UGOAttributeSetBase, Damage)
		
	UFUNCTION(BlueprintCallable)
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	UFUNCTION(BlueprintCallable)
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	UFUNCTION(BlueprintCallable)
	virtual void OnRep_Mana(const FGameplayAttributeData& oldMana);
	UFUNCTION(BlueprintCallable)
	virtual void OnRep_MaxMana(const FGameplayAttributeData& oldMaxMana);


	UFUNCTION(BlueprintCallable)
	virtual void OnRep_Strength(const FGameplayAttributeData& oldStrength);
	UFUNCTION(BlueprintCallable)
	virtual void OnRep_Intelligence(const FGameplayAttributeData& oldIntelligence);
	UFUNCTION(BlueprintCallable)
	virtual void OnRep_Resilience(const FGameplayAttributeData& oldResilience);
	UFUNCTION(BlueprintCallable)
	virtual void OnRep_Vigor(const FGameplayAttributeData& oldVigor);


	UFUNCTION(BlueprintCallable)
	virtual void OnRep_Armor(const FGameplayAttributeData& oldArmor);
	UFUNCTION(BlueprintCallable)
	virtual void OnRep_ArmorPenetration(const FGameplayAttributeData& oldArmorPenetration);
	UFUNCTION(BlueprintCallable)
	virtual void OnRep_CriticalHitChance(const FGameplayAttributeData& oldCriticalHitChance);
	UFUNCTION(BlueprintCallable)
	virtual void OnRep_CriticalHitDamage(const FGameplayAttributeData& oldCriticalHitDamage);
	UFUNCTION(BlueprintCallable)
	virtual void OnRep_CriticalHitResistance(const FGameplayAttributeData& oldCriticalHitResistance);
	UFUNCTION(BlueprintCallable)
	virtual void OnRep_HealthRegeneration(const FGameplayAttributeData& oldHealthRegeneration);
	UFUNCTION(BlueprintCallable)
	virtual void OnRep_ManaRegeneration(const FGameplayAttributeData& oldManaRegeneration);
	UFUNCTION(BlueprintCallable)
	virtual void OnRep_BlockChance(const FGameplayAttributeData& oldBlockChance);
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const override;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
};

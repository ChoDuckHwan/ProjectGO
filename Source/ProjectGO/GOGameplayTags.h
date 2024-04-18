// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
/**
 *	GOGameplayTags
 *
 *	Singleton containing native Gameplay Tags
 */
class PROJECTGO_API GOGameplayTags
{
public:

};

struct FGOGameplayTags
{
public:
	static const FGOGameplayTags& Get() { return  GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;

	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;

	FGameplayTag InputTag_AbilityQ;
	FGameplayTag InputTag_AbilityW;
	FGameplayTag InputTag_AbilityR;

	FGameplayTag InputTag_Item1;
	FGameplayTag InputTag_Item2;
	FGameplayTag InputTag_Item3;
	FGameplayTag InputTag_Item4;
	FGameplayTag InputTag_Item5;
	FGameplayTag InputTag_Item6;


private:
	static FGOGameplayTags GameplayTags;
};
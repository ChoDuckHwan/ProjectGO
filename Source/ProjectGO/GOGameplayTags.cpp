// Fill out your copyright notice in the Description page of Project Settings.


#include "GOGameplayTags.h"
#include "GameplayTagsManager.h"

FGOGameplayTags FGOGameplayTags::GameplayTags;

void FGOGameplayTags::InitializeNativeGameplayTags()
{
	//called UGOAssetManager::StartInitialLoading()
	/*
	 * Primary Attribute
	 */
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"), FString("Increases physical damage"));
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"), FString("Increases magical damage"));
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"), FString("Increases armor and armor penetration"));
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"), FString("Increases health"));	

	/*
	 * Secondary Attribute
	 */

	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces damage taken, improves block chance"));
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"), FString("Ignore percentage of enemy armor, increases critical hit chance"));
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"), FString("Chance to cut incoming damage in half"));
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"), FString("Chance to double damage plus critical hit bonus"));
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"), FString("Bonus damage added when a critical hit is scored"));
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"), FString("Reduces critical hit chance of attacking enemies"));
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"), FString("Amount of health regenerated every 1 second"));
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"), FString("Amount of mana regenerated every 1 second"));

	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"), FString("max health"));
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"), FString("max Mana"));

	/*Input Tags
	 *
	 */

	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("LeftMouse Button"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("RightMouse Button"));
	GameplayTags.InputTag_AbilityQ = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.AbilityQ"), FString("AbilityQ Button"));
	GameplayTags.InputTag_AbilityW = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.AbilityW"), FString("AbilityW Button"));
	GameplayTags.InputTag_AbilityR = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.AbilityR"), FString("AbilityR Button"));

	GameplayTags.InputTag_Item1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Item1"), FString("1 Item Use Button"));
	GameplayTags.InputTag_Item2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Item2"), FString("2 Item Use Button"));
	GameplayTags.InputTag_Item3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Item3"), FString("3 Item Use Button"));
	GameplayTags.InputTag_Item4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Item4"), FString("4 Item Use Button"));
	GameplayTags.InputTag_Item5 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Item5"), FString("5 Item Use Button"));
	GameplayTags.InputTag_Item6 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Item6"), FString("6 Item Use Button"));


	/*
	 * Damage
	 */

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("Damage"));
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"), FString("Fire Damage Fire Type"));
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"), FString("Fire Damage Lightning Type"));
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"), FString("Fire Damage Arcane Type"));
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"), FString("Fire Damage Physical Type"));

	/*
	 *	Resistances
	 */
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"), FString("Attributes.Resistance.Fire Type"));
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"), FString("Attributes.Resistance.Lightning Type"));
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"), FString("Attributes.Resistance.Arcane"));
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"), FString("Attributes.Resistance.Physical"));
	
	/*
	 * Hit React
	 */
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("Tag Granted when Hit React"));

	/*
	 *	Abilities
	 */
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"), FString("Abilities Attack"));


	GameplayTags.Montage_Attack_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.Weapon"), FString("Attack Weapon"));
	GameplayTags.Montage_Attack_Weapon_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.Weapon.Right"), FString("Right Weapon Attack"));
	GameplayTags.Montage_Attack_Weapon_Left = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.Weapon.Left"), FString("Left Weapon Attack"));

	GameplayTags.Montage_Attack_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.LeftHand"), FString("Left Hand Attack"));
	GameplayTags.Montage_Attack_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.RightHand"), FString("Right Hand Attack"));

	/*
	 *	Map of GameplayTags / Resistance GameplayTags
	 */
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
}

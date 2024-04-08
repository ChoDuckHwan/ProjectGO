// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Character/Abilities/AttributeSet/GOAttributeSetBase.h"
#include "Net/UnrealNetwork.h"

void UGOAttributeSetBase::OnRep_Level(const FGameplayAttributeData& OldLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, Level, OldLevel);	
}

void UGOAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, Health, OldHealth);
}

void UGOAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, MaxHealth, OldMaxHealth);
}

void UGOAttributeSetBase::OnRep_Mana(const FGameplayAttributeData& oldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, Mana, oldMana);
}

void UGOAttributeSetBase::OnRep_MaxMana(const FGameplayAttributeData& oldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, MaxMana, oldMaxMana);
}

void UGOAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, MaxMana, COND_None, REPNOTIFY_Always);


}

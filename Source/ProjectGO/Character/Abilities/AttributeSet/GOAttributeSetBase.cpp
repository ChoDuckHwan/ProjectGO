// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Character/Abilities/AttributeSet/GOAttributeSetBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

#include "Net/UnrealNetwork.h"
#include "ProjectGO/GOGameplayTags.h"
#include "ProjectGO/Character/Abilities/GOAbilityBFL.h"
#include "ProjectGO/Interaction/CombatInterface.h"
#include "ProjectGO/Interaction/PlayerInterface.h"
#include "ProjectGO/PlayerController/ProjectGOPlayerController.h"

UGOAttributeSetBase::UGOAttributeSetBase()
{
	const FGOGameplayTags& GameplayTags = FGOGameplayTags::Get();

	/* Primary Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);	
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);


	/* Secondary Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);

	/* resistance attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Arcane, GetArcaneResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire, GetFireResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Lightning, GetLightningResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);

}

void UGOAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	//Conditional Check
	//just return temporary changed value. not change primitive attribute's value
	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UGOAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	//only server
	Super::PostGameplayEffectExecute(Data);
	FEffectProperties EffectProperties;
	SetEffectProperties(Data, EffectProperties);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if(Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if(LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			const bool bFatal = NewHealth <= 0.f;
			if(!bFatal)
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FGOGameplayTags::Get().Effects_HitReact);
				EffectProperties.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
			else
			{
				if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProperties.TargetAvatarActor))
				{
					CombatInterface->Die();
				}
				SendXPEvent(EffectProperties);
			}
			bool bBlocked = UGOAbilityBFL::IsBlockedHit(EffectProperties.GameplayEffectContextHandle); 
			bool bCriticalHit = UGOAbilityBFL::IsCriticalHit(EffectProperties.GameplayEffectContextHandle); 

			ShowFloatText(EffectProperties, LocalIncomingDamage, bBlocked, bCriticalHit);
		}
	}

	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		const float LocalIncomingXP = GetIncomingXP();
		SetIncomingXP(0.f);
		//UE_LOG(LogTemp, Log, TEXT("Incomming XP : %f"), LocalIncomingXP);

		//TODO : See if we should level up

		//source character is the owner, since GA_ListenForEvents aplies GE_EventBasedEffect, adding to IncomingXP
		if(EffectProperties.SourceCharacter->Implements<UPlayerInterface>() && EffectProperties.SourceCharacter->Implements<UCombatInterface>())
		{
			const int32 CurrentLevel = ICombatInterface::Execute_GetLevel(EffectProperties.SourceCharacter);
			const int32 CurrentXP = IPlayerInterface::Execute_GetXP(EffectProperties.SourceCharacter);
			
			const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(EffectProperties.SourceCharacter, CurrentXP + LocalIncomingXP); 
			const int32 NumLevelUps = NewLevel - CurrentLevel;

			if(NumLevelUps > 0 )
			{				
				const int32 AttributePointReward =IPlayerInterface::Execute_GetAttributePointsReward(EffectProperties.SourceCharacter, CurrentLevel, NewLevel);
				const int32 SpellPointReward = IPlayerInterface::Execute_GetSpellPointsReward(EffectProperties.SourceCharacter, CurrentLevel, NewLevel);

				IPlayerInterface::Execute_AddToPlayerLevel(EffectProperties.SourceCharacter, NumLevelUps);
				IPlayerInterface::Execute_AddToSpellPoints(EffectProperties.SourceCharacter, AttributePointReward);
				IPlayerInterface::Execute_AddToAttributePoints(EffectProperties.SourceCharacter, SpellPointReward);

				IPlayerInterface::Execute_LevelUp(EffectProperties.SourceCharacter);

				bTopOffHealth = true;
				bTopOffMana = true;		
			}
			IPlayerInterface::Execute_AddToXP(EffectProperties.SourceCharacter, LocalIncomingXP);
		}
	}
}

void UGOAttributeSetBase::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{	
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if(Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		bTopOffHealth = false;
		SetHealth(GetMaxHealth());
	}

	if(Attribute == GetManaAttribute() && bTopOffMana)
	{
		bTopOffMana = false;
		SetMana(GetMaxMana());
	}
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

void UGOAttributeSetBase::OnRep_Strength(const FGameplayAttributeData& oldStrength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, Strength, oldStrength);
}

void UGOAttributeSetBase::OnRep_Intelligence(const FGameplayAttributeData& oldIntelligence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, Intelligence, oldIntelligence);

}

void UGOAttributeSetBase::OnRep_Resilience(const FGameplayAttributeData& oldResilience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, Resilience, oldResilience);

}

void UGOAttributeSetBase::OnRep_Vigor(const FGameplayAttributeData& oldVigor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, Vigor, oldVigor);
}

void UGOAttributeSetBase::OnRep_Armor(const FGameplayAttributeData& oldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, Armor, oldArmor);
}

void UGOAttributeSetBase::OnRep_ArmorPenetration(const FGameplayAttributeData& oldArmorPenetration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, ArmorPenetration, oldArmorPenetration);
}

void UGOAttributeSetBase::OnRep_CriticalHitChance(const FGameplayAttributeData& oldCriticalHitChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, CriticalHitChance, oldCriticalHitChance);
}

void UGOAttributeSetBase::OnRep_CriticalHitDamage(const FGameplayAttributeData& oldCriticalHitDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, CriticalHitDamage, oldCriticalHitDamage);
}

void UGOAttributeSetBase::OnRep_CriticalHitResistance(const FGameplayAttributeData& oldCriticalHitResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, CriticalHitResistance, oldCriticalHitResistance);
}

void UGOAttributeSetBase::OnRep_HealthRegeneration(const FGameplayAttributeData& oldHealthRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, HealthRegeneration, oldHealthRegeneration);
}

void UGOAttributeSetBase::OnRep_ManaRegeneration(const FGameplayAttributeData& oldManaRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, ManaRegeneration, oldManaRegeneration);
}

void UGOAttributeSetBase::OnRep_BlockChance(const FGameplayAttributeData& oldBlockChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, BlockChance, oldBlockChance);

}

void UGOAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, MaxMana, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, BlockChance, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, Vigor, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGOAttributeSetBase, PhysicalResistance, COND_None, REPNOTIFY_Always);

}

void UGOAttributeSetBase::OnRep_FireResistance(const FGameplayAttributeData& oldFireResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, FireResistance, oldFireResistance);
}

void UGOAttributeSetBase::OnRep_LightningResistance(const FGameplayAttributeData& oldLightningResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, LightningResistance, oldLightningResistance);
}

void UGOAttributeSetBase::OnRep_ArcaneResistance(const FGameplayAttributeData& oldArcaneResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, ArcaneResistance, oldArcaneResistance);
}

void UGOAttributeSetBase::OnRep_PhysicalResistance(const FGameplayAttributeData& oldPhysicalResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGOAttributeSetBase, PhysicalResistance, oldPhysicalResistance);
}

void UGOAttributeSetBase::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	//Source == Cursor of the effect, Target = Target of the offset(owner of this AS)

	Props.GameplayEffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.GameplayEffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && IsValid(Props.SourceASC->GetAvatarActor()))
	{
		Props.SourceAvatarActor = Props.SourceASC->GetAvatarActor();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController != nullptr)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UGOAttributeSetBase::ShowFloatText(const FEffectProperties& Properties, float HitDamage, bool bBlockedHit, bool bCriticalHit) const
{
	if (Properties.SourceCharacter != Properties.TargetCharacter)
	{		
		if (AProjectGOPlayerController* PC = Cast<AProjectGOPlayerController>(Properties.SourceCharacter->GetController()))
		{
			PC->ShowDamageNumber(HitDamage, Properties.TargetCharacter, bBlockedHit, bCriticalHit);
			return;
		}
		if (AProjectGOPlayerController* PC = Cast<AProjectGOPlayerController>(Properties.TargetCharacter->GetController()))
		{
			PC->ShowDamageNumber(HitDamage, Properties.TargetCharacter, bBlockedHit, bCriticalHit);
			return;
		}
	}
}

void UGOAttributeSetBase::SendXPEvent(const FEffectProperties& Properties)
{
	if(Properties.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetLevel(Properties.TargetCharacter);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Properties.TargetCharacter);

		const int32 XPReward = UGOAbilityBFL::GetXPRewardForClassAndLevel(Properties.TargetCharacter, TargetClass, TargetLevel);

		const FGOGameplayTags GameplayTag = FGOGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTag.Attributes_Meta_incomingXP;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Properties.SourceCharacter, GameplayTag.Attributes_Meta_incomingXP, Payload);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Character/Abilities/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "ProjectGO/GOGameplayTags.h"
#include "ProjectGO/Character/Abilities/GOAbilityBFL.h"
#include "ProjectGO/Character/Abilities/GOAbilityTypes.h"
#include "ProjectGO/Character/Abilities/Data/CharacterClassInfo.h"
#include "ProjectGO/Interaction/CombatInterface.h"

static const GODamageStatics& DamageStatics()
{
	static GODamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	//Capture Attributes
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);

}


void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* Target_ASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = Target_ASC ? Target_ASC->GetAvatarActor() : nullptr;

	const ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	const ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters AggregatorEvaluateParameters;
	AggregatorEvaluateParameters.TargetTags = TargetTag;
	AggregatorEvaluateParameters.SourceTags = SourceTag;

	//Get Damage Set by Caller Magnitude
	float Damage = 0.f;
	for(const auto& DamageTypePair : FGOGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = DamageTypePair.Key;
		const FGameplayTag ResistanceTag = DamageTypePair.Value;
		
		checkf(GODamageStatics().TagsToCaptureDefs.Contains(ResistanceTag), TEXT("Tags to capturedef doesn't contain tag : [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition  CaptureDef = GODamageStatics().TagsToCaptureDefs[ResistanceTag];
		
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag);

		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, AggregatorEvaluateParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= (100.f - Resistance ) / 100.f;
		Damage += DamageTypeValue;
	}
	
	//Capture BlockChance on Target, and determine if there was a successful Blocks
	//If Block, half damage.
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, AggregatorEvaluateParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	Damage = bBlocked ? Damage * 0.5f : Damage;

	//ArmorPenetration ignores a percentage of the Target's Armor.
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, AggregatorEvaluateParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	const UCharacterClassInfo* CharacterClassInfo = UGOAbilityBFL::GetCharacterClassInfo(SourceAvatar);
	if(!CharacterClassInfo) return;

	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	if(!ArmorPenetrationCurve) return;
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetLevel());
	
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	if(!EffectiveArmorCurve) return;
	const float EffectiveArmorCurveCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetLevel());

	float SourceArmorPenetartion = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, AggregatorEvaluateParameters, SourceArmorPenetartion);
	SourceArmorPenetartion = FMath::Max<float>(SourceArmorPenetartion, 0.f);
	
	const float EffectiveArmor = TargetArmor * ( 100.f - SourceArmorPenetartion * ArmorPenetrationCoefficient) / 100.f;

	Damage *= (100.f - EffectiveArmor * EffectiveArmorCurveCoefficient) / 100.f;

	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, AggregatorEvaluateParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

	float TargetCriticalHitResistence = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, AggregatorEvaluateParameters, TargetCriticalHitResistence);
	TargetCriticalHitResistence = FMath::Max<float>(TargetCriticalHitResistence, 0.f);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, AggregatorEvaluateParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	//Critical Hit Resistance reduces critical hit chance by percentage.
	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	if(!CriticalHitResistanceCurve) return;
	const float CriticalHitResistance = CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetLevel());
	
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistence * CriticalHitResistance;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;
	
	//double damage plus a if critical hit.
	Damage = bCriticalHit ? Damage * 2.f + SourceCriticalHitDamage : Damage;

	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
	UGOAbilityBFL::SetIsBlockedHit(ContextHandle, bBlocked);
	UGOAbilityBFL::SetIsCriticalHit(ContextHandle, bCriticalHit);

	const FGameplayModifierEvaluatedData EvaluatedData(UGOAttributeSetBase::GetIncomingDamageAttribute(),
	                                                   EGameplayModOp::Additive, Damage);
	
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectGO/Character/Abilities/ModMagCalc/MMC_MaxMana.h"
#include "ProjectGO/Character/Abilities/AttributeSet/GOAttributeSetBase.h"
#include "ProjectGO/Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UGOAttributeSetBase::GetVigorAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluateParameters, Intelligence);

	Intelligence = FMath::Max(Intelligence, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const float Level = CombatInterface->GetLevel();
	return 50.f + 2.5f * Intelligence + 15.f * Level;
}

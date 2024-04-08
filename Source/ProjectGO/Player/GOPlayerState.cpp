// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Player/GOPlayerState.h"
#include "ProjectGO/Character/Abilities/GOAbilitySystemComponent.h"
#include "ProjectGO/Character/Abilities/AttributeSet/GOAttributeSetBase.h"

AGOPlayerState::AGOPlayerState()
{	
	AbilitySystemComponent = CreateDefaultSubobject<UGOAbilitySystemComponent>(TEXT("GOAbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSetBase = CreateDefaultSubobject<UGOAttributeSetBase>(TEXT("GOAttributeSetBase"));
	NetUpdateFrequency = 100.0f;
}

void AGOPlayerState::BeginPlay()
{
	Super::BeginPlay();	
}

UAbilitySystemComponent* AGOPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UGOAttributeSetBase* AGOPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}
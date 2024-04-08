// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Character/ProjectGOMonster.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GOAbilitySystemComponent.h"
#include "Abilities/AttributeSet/GOAttributeSetBase.h"

AProjectGOMonster::AProjectGOMonster(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MonsterAbilitySystemComponent = CreateDefaultSubobject<UGOAbilitySystemComponent>(TEXT("MonsterAbilitySystemComponent"));
	MonsterAbilitySystemComponent->SetIsReplicated(true);
	MonsterAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	MonsterAttributeSetBase = CreateDefaultSubobject<UGOAttributeSetBase>(TEXT("MonsterAttributeSetBase"));

	AbilitySystemComponent = MonsterAbilitySystemComponent;
	AttributeSetBase = MonsterAttributeSetBase;
}

void AProjectGOMonster::BeginPlay()
{
	Super::BeginPlay();
	if(GetNetMode() != NM_Client)
	{
		AddStartupEffects();
		AddCharacterAbilities();
	}
	InitializeAbilityValue(nullptr);
}

void AProjectGOMonster::InitializeAbilityValue(AGOPlayerState* PS)
{
	AbilitySystemComponent = MonsterAbilitySystemComponent;
	AttributeSetBase = MonsterAttributeSetBase;
	if (!AbilitySystemComponent.IsValid() || !AttributeSetBase.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent, AttributeSetBase Is Not Valid"));
		return;
	}
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	Super::InitializeAbilityValue(PS);

}

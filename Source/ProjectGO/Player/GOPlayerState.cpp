// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Player/GOPlayerState.h"
#include "Net/UnrealNetwork.h"
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

void AGOPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGOPlayerState, Level);
}

void AGOPlayerState::BeginPlay()
{
	Super::BeginPlay();	
}

void AGOPlayerState::OnRep_Level(const int32& oldLevel)
{
}

UAbilitySystemComponent* AGOPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UGOAttributeSetBase* AGOPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

int32 AGOPlayerState::GetPlayerLevel() const
{
	return Level;
}

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
	DOREPLIFETIME(AGOPlayerState, XP);
	DOREPLIFETIME(AGOPlayerState, AttributePoints);
	DOREPLIFETIME(AGOPlayerState, SpellPoints);

}

void AGOPlayerState::AddXP(const int32& PlusXP)
{
	SetXP(XP + PlusXP);
}

void AGOPlayerState::AddToLevel(const int32& PlusLevel)
{
	SetLevel(Level + PlusLevel);
}

void AGOPlayerState::AddToAttributePoints(const int32& AddValue)
{
	AttributePoints += AddValue;
	AttributePointChangedDelegate.Broadcast(AttributePoints);
}

void AGOPlayerState::AddToSpellPoints(const int32& AddValue)
{
	SpellPoints += AddValue;
	SpellPointChangedDelegate.Broadcast(SpellPoints);
}

void AGOPlayerState::SetXP(const int32& NewXP)
{
	XP = NewXP;
	XPChangedDelegate.Broadcast(XP);
}

void AGOPlayerState::SetLevel(const int32& NewLevel)
{
	Level = NewLevel;
	LevelChangedDelegate.Broadcast(Level);
}

void AGOPlayerState::BeginPlay()
{
	Super::BeginPlay();	
}

void AGOPlayerState::OnRep_Level(const int32& oldLevel)
{
	LevelChangedDelegate.Broadcast(Level);
}

void AGOPlayerState::OnRep_XP(const int32& oldXP)
{
	XPChangedDelegate.Broadcast(XP);
}

void AGOPlayerState::OnRep_AttributePoints(const int32& OldValue)
{
	AttributePointChangedDelegate.Broadcast(AttributePoints);
}

void AGOPlayerState::OnRep_SpellPoints(const int32& OldValue)
{
	SpellPointChangedDelegate.Broadcast(SpellPoints);
}

UAbilitySystemComponent* AGOPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UGOAttributeSetBase* AGOPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

UAttributeSet* AGOPlayerState::GetAttributeSet() const
{
	return AttributeSetBase;
}


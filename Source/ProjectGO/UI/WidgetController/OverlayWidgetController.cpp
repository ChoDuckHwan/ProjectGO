// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/UI/WidgetController/OverlayWidgetController.h"
#include "ProjectGO/Character/Abilities/AttributeSet/GOAttributeSetBase.h"

void UOverlayWidgetController::BroadcastInitValue()
{
	if(const UGOAttributeSetBase* GOAttributeSet = Cast<UGOAttributeSetBase>(AttributeSet))
	{
		 OnHealthChanged.Broadcast(GOAttributeSet->GetHealth());
		 OnMaxHealthChanged.Broadcast(GOAttributeSet->GetMaxHealth());
		 OnManaChanged.Broadcast(GOAttributeSet->GetMana());
		 OnMaxManaChanged.Broadcast(GOAttributeSet->GetMaxMana());
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	if (const UGOAttributeSetBase* GOAttributeSet = Cast<UGOAttributeSetBase>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GOAttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::HealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GOAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &ThisClass::MaxHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GOAttributeSet->GetManaAttribute()).AddUObject(this, &ThisClass::ManaChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GOAttributeSet->GetMaxManaAttribute()).AddUObject(this, &ThisClass::MaxManaChanged);
		
		//AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GOAttributeSet->GetLevelAttribute()).AddUObject(this, &ThisClass::CharacterLevelChanged);
	}
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data)
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data)
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/UI/WidgetController/OverlayWidgetController.h"
#include "ProjectGO/Character/Abilities/GOAbilitySystemComponent.h"
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

		Cast<UGOAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda([&](const FGameplayTagContainer& AssetTags)
			{
				for (const auto& AssetTag : AssetTags)
				{
					// *"Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False					
					if(AssetTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Message"))))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, AssetTag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}

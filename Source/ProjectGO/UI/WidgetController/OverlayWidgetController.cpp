// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/UI/WidgetController/OverlayWidgetController.h"
#include "ProjectGO/Character/Abilities/GOAbilitySystemComponent.h"
#include "ProjectGO/Character/Abilities/AttributeSet/GOAttributeSetBase.h"
#include "ProjectGO/Player/GOPlayerState.h"

void UOverlayWidgetController::BroadcastInitValue()
{
	if(const UGOAttributeSetBase* GOAttributeSet = Cast<UGOAttributeSetBase>(AttributeSet.Get()))
	{
		 OnHealthChanged.Broadcast(GOAttributeSet->GetHealth());
		 OnMaxHealthChanged.Broadcast(GOAttributeSet->GetMaxHealth());
		 OnManaChanged.Broadcast(GOAttributeSet->GetMana());
		 OnMaxManaChanged.Broadcast(GOAttributeSet->GetMaxMana());
	}

	if (UGOAbilitySystemComponent* GOASC = Cast<UGOAbilitySystemComponent>(AbilitySystemComponent.Get()))
	{
		if (GOASC->bAbilitiesGiven)
		{
			OnInitializeStartupAbilities();
		}
		else
		{
			GOASC->AbilitiesGivenDelegate.AddUObject(this, &ThisClass::OnInitializeStartupAbilities);
		}
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	if (const UGOAttributeSetBase* GOAttributeSet = Cast<UGOAttributeSetBase>(AttributeSet.Get()))
	{
		AbilitySystemComponent.Get()->GetGameplayAttributeValueChangeDelegate(GOAttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::HealthChanged);
		AbilitySystemComponent.Get()->GetGameplayAttributeValueChangeDelegate(GOAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &ThisClass::MaxHealthChanged);
		AbilitySystemComponent.Get()->GetGameplayAttributeValueChangeDelegate(GOAttributeSet->GetManaAttribute()).AddUObject(this, &ThisClass::ManaChanged);
		AbilitySystemComponent.Get()->GetGameplayAttributeValueChangeDelegate(GOAttributeSet->GetMaxManaAttribute()).AddUObject(this, &ThisClass::MaxManaChanged);
		
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
	if(AGOPlayerState* GPS = CastChecked<AGOPlayerState>(PlayerState))
	{
		GPS->XPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
		GPS->LevelChangedDelegate.AddUObject(this, &ThisClass::OnLevelChanged);
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

void UOverlayWidgetController::OnInitializeStartupAbilities()
{
	if(!AbilitySystemComponent.Get())
	{
		return;
	}
	UGOAbilitySystemComponent* GOASC = Cast<UGOAbilitySystemComponent>(AbilitySystemComponent);
	if(!GOASC)
	{
		return;
	}
	if(!GOASC->bAbilitiesGiven)
	{
		return;
	}

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([&](const FGameplayAbilitySpec& GameplayAbilitySpec)
	{		
		FGOAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(GOASC->GetAbilityTagFromSpec(GameplayAbilitySpec));
		Info.InputTag = GOASC->GetInputTagFromSpec(GameplayAbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);		
	});
	GOASC->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(const int32& NewXP)
{
	if(AGOPlayerState* GPS = CastChecked<AGOPlayerState>(PlayerState))
	{
		checkf(GPS->LevelupInfo, TEXT("Please fill out GOPlayerState Blueprint"));
		if(const ULevelupInfo* LevelupInfo = GPS->LevelupInfo)
		{
			const int32 Level = LevelupInfo->FindLevelForXP(NewXP);
			const int32 MaxLevel = LevelupInfo->LevelupInfos.Num();

			if(Level <= MaxLevel && Level > 0)
			{
				const int32 LevelupRequirement = LevelupInfo->LevelupInfos[Level].LevelupRequirement;
				const int32 PreviousLevelupRequirement = LevelupInfo->LevelupInfos[Level-1].LevelupRequirement;

				const int32 DeltaLevelupRequirement = LevelupRequirement - PreviousLevelupRequirement;
				const int32 XPForThisLevel = NewXP - PreviousLevelupRequirement;

				const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelupRequirement);

				OnPlayerXPChanged.Broadcast(XPBarPercent);
			}
		}
	}	
}

void UOverlayWidgetController::OnLevelChanged(const int32& NewLevel)
{
	OnPlayerLevelChanged.Broadcast(NewLevel);
}

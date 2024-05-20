// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Character/Abilities/AsyncTasks/WaitCooldownChange_Async.h"

UWaitCooldownChange_Async* UWaitCooldownChange_Async::WaitForCooldownChange(UAbilitySystemComponent* ASC, const FGameplayTag& InCooldownTag)
{
	TWeakObjectPtr<UWaitCooldownChange_Async> WaitCooldownChange = NewObject<UWaitCooldownChange_Async>();
	WaitCooldownChange->ASC = ASC;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	if(!IsValid(ASC) || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->Endtask();
		return nullptr;
	}

	//To know when a cooldown has ended
	ASC->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(WaitCooldownChange.Get(), &UWaitCooldownChange_Async::CooldownTagChanged);

	//To know when a cooldown effect has been applied
	ASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange.Get(), &UWaitCooldownChange_Async::OnActiveEffectAdded);
	
	return WaitCooldownChange.Get();
}

void UWaitCooldownChange_Async::Endtask()
{
	if(!ASC.IsValid())
	{
		return;
	}
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCooldownChange_Async::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if(NewCount == 0 && InCooldownTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *InCooldownTag.ToString());
		CooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange_Async::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC,	const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle EffectHandleActive)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);
	
	if(AssetTags.HasTag(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		if(TimesRemaining.Num() > 0)
		{
			float TimeRemaining = TimesRemaining[0];
			for(int32 i = 0; i<TimesRemaining.Num(); ++i)
			{
				if(TimesRemaining[i] > TimeRemaining)
				{
					TimeRemaining = TimesRemaining[i];
				}
			}
			CooldownStart.Broadcast(TimeRemaining);
		}
	}
}

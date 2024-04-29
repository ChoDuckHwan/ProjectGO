// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Character/Abilities/GOAbilityBFL.h"

#include "GOAbilityTypes.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectGO/GameMode/ProjectGOGameMode.h"
#include "ProjectGO/HUD/InGameHud.h"
#include "ProjectGO/Player/GOPlayerState.h"
#include "ProjectGO/UI/GOWidgetController.h"

UOverlayWidgetController* UGOAbilityBFL::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if(AInGameHud* GoHud = Cast<AInGameHud>(PC->GetHUD()))
		{
			AGOPlayerState* PlayerState = PC->GetPlayerState<AGOPlayerState>();
			check(PlayerState);
			return GoHud->GetOverlayWidgetController(FWidgetControllerParams(PC, PlayerState, PlayerState->GetAbilitySystemComponent(), PlayerState->GetAttributeSet()));
		}
	}
	return nullptr;
}

UGOAttributeWidgetController* UGOAbilityBFL::GetAttributeWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AInGameHud* GoHud = Cast<AInGameHud>(PC->GetHUD()))
		{
			AGOPlayerState* PlayerState = PC->GetPlayerState<AGOPlayerState>();
			check(PlayerState);
			return GoHud->GetAttributeWidgetController(FWidgetControllerParams(PC, PlayerState, PlayerState->GetAbilitySystemComponent(), PlayerState->GetAttributeSet()));
		}
	}
	return nullptr;
}

void UGOAbilityBFL::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (!CharacterClassInfo) return;

	AActor* AvatorActor = ASC->GetAvatarActor();
	const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	FGameplayEffectContextHandle PrimaryContextHandle = ASC->MakeEffectContext();
	PrimaryContextHandle.AddSourceObject(AvatorActor);

	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryContextHandle = ASC->MakeEffectContext();
	SecondaryContextHandle.AddSourceObject(AvatorActor);
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalContextHandle = ASC->MakeEffectContext();
	VitalContextHandle.AddSourceObject(AvatorActor);
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}

void UGOAbilityBFL::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{	
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(!CharacterClassInfo) return;
	for(const auto& Abilitiy : CharacterClassInfo->Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Abilitiy, 1);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UGOAbilityBFL::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AProjectGOGameMode* GameMode = Cast<AProjectGOGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!GameMode) return nullptr;

	return GameMode->CharacterClassInfo;
}

bool UGOAbilityBFL::IsBlockedHit(const FGameplayEffectContextHandle& EffectContext)
{
	if(const FGOGameplayEffectContext* GOGameplayEffectContext = static_cast<const FGOGameplayEffectContext*>(EffectContext.Get()))
	{
		return GOGameplayEffectContext->IsBlockedHit();
	}
	return false;
}

bool UGOAbilityBFL::IsCriticalHit(const FGameplayEffectContextHandle& EffectContext)
{
	if(const FGOGameplayEffectContext* GOGameplayEffectContext = static_cast<const FGOGameplayEffectContext*>(EffectContext.Get()))
	{
		return GOGameplayEffectContext->IsCriticalHit();
	}
	return false;
}

void UGOAbilityBFL::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContext, bool InIsBlockedHit)
{
	if(FGOGameplayEffectContext* GOGameplayEffectContext = static_cast<FGOGameplayEffectContext*>(EffectContext.Get()))
	{
		GOGameplayEffectContext->SetIsBlockedHit(InIsBlockedHit);
	}
}

void UGOAbilityBFL::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContext, bool bInIsCriticalHit)
{
	if(FGOGameplayEffectContext* GOGameplayEffectContext = static_cast<FGOGameplayEffectContext*>(EffectContext.Get()))
    	{
    		GOGameplayEffectContext->SetIsCriticalHit(bInIsCriticalHit);
    	}
}

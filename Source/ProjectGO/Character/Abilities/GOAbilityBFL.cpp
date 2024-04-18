// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Character/Abilities/GOAbilityBFL.h"
#include "Kismet/GameplayStatics.h"
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

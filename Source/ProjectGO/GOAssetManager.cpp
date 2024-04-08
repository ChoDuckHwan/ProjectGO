// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAssetManager.h"
#include "AbilitySystemGlobals.h"

void UGOAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}

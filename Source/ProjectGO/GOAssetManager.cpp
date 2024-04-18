// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "GOGameplayTags.h"

void UGOAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
	FGOGameplayTags::InitializeNativeGameplayTags();
}

UGOAssetManager& UGOAssetManager::Get()
{
	check(GEngine);
	UGOAssetManager* AssetManager = Cast<UGOAssetManager>(GEngine->AssetManager);
	return *AssetManager;
}

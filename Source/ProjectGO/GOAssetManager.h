// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GOAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGO_API UGOAssetManager : public UAssetManager
{
	GENERATED_BODY()

protected:
	virtual void StartInitialLoading() override;

public:
	static UGOAssetManager& Get();
};

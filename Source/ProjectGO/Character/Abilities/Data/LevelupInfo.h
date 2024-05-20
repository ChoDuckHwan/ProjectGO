// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelupInfo.generated.h"

USTRUCT()
struct FGOLevelupInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 LevelupRequirement = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 1;

	UPROPERTY(EditDefaultsOnly)
	int32 SkillPointAward = 1;
};

/**
 * 
 */
UCLASS()
class PROJECTGO_API ULevelupInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FGOLevelupInfo> LevelupInfos;

	int32 FindLevelForXP(int32 XP) const;
};

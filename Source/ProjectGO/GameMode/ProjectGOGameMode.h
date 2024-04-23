// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ProjectGOGameMode.generated.h"

class UCharacterClassInfo;

UCLASS(minimalapi)
class AProjectGOGameMode : public AGameMode
{
	GENERATED_BODY()	

public:
	AProjectGOGameMode();

	UPROPERTY(EditDefaultsOnly, Category = "CharacterClassDefaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
};




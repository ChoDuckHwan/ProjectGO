// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTGO_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public :
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(const int32& XP);

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(const int32& InXP) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 CurrentLevel, int32 NewLevel) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(int32 CurrentLevel, int32 NewLevel) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints() const;
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 InPlayerLevel);

	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(int32 InAttributePoint);

	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoints(int32 InSpellPoint);
	
public:
};

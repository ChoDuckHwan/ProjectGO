// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ProjectGO/PathFind/PathFinderVolume.h"
#include "GOGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGO_API AGOGameState : public AGameState
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TObjectPtr<APathFinderVolume> PathFinderVolume;

public:
	UFUNCTION(BlueprintCallable)
	const APathFinderVolume* GetPathFinderVolume() const;

	UFUNCTION(BlueprintCallable)
	bool SetPathFinderVolume(APathFinderVolume* NewPathFinderVolume);

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void UpdatePathFindGridCanMoveByLocation(const FVector2D& Location, bool bCanMove);
};

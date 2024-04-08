// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/GameState/GOGameState.h"

#include "Net/UnrealNetwork.h"

const APathFinderVolume* AGOGameState::GetPathFinderVolume() const
{
	return PathFinderVolume;
}

bool AGOGameState::SetPathFinderVolume(APathFinderVolume* NewPathFinderVolume)
{
	if(NewPathFinderVolume)
	PathFinderVolume = NewPathFinderVolume;
	return true;
}

void AGOGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);	
}

void AGOGameState::UpdatePathFindGridCanMoveByLocation(const FVector2D& Location, bool bCanMove)
{
	if(!PathFinderVolume)
	{
		return;
	}
	PathFinderVolume->UpdateGridArrayByLocation(Location, bCanMove);
}

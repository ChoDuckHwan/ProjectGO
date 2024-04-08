// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/PathFind/PathFinderVolume.h"
#include "PathFinderComponent.h"


// Sets default values
APathFinderVolume::APathFinderVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APathFinderVolume::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void APathFinderVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

//void APathFinderVolume::OnRep_ChangeGridIdxList()
//{
//	if(!ChangeGridIdxList.IsEmpty() && GetNetMode() == ENetMode::NM_Client)
//	{
//		for(const auto&  ChangeGridIdx: ChangeGridIdxList)
//		{
//			Grids[ChangeGridIdx].SetReverseCanMove();
//		}
//	}
//	ChangeGridIdxList.Empty();
//}

void APathFinderVolume::AddGrid(const FPathFindGridInfo& NewGrid)
{
	Grids.Add(NewGrid);
}

void APathFinderVolume::SetGrids(const TArray<FPathFindGridInfo>& NewGrids)
{
	Grids = NewGrids;
}

void APathFinderVolume::ResetGrid()
{
	Grids.Reset();
}

const TArray<FPathFindGridInfo>& APathFinderVolume::GetGrids() const
{
	return Grids;
}

int32 APathFinderVolume::GetGridArrayIndexByLocation(const FVector2D& Location) const
{
	const FPathFindGridInfo FirstGrid = Grids[0];
	const FPathFindGridInfo LastGrid = Grids[Grids.Num() - 1];
	int Index = -1;
	if (Location.ComponentwiseAllGreaterOrEqual(FirstGrid.GetWorldLocation()) && Location.ComponentwiseAllLessOrEqual(LastGrid.GetWorldLocation()))
	{
		const int row = Location.X / (GridSize.X * 2);
		const int col = Location.Y / (GridSize.Y * 2);
		Index = ((MapSize.Y/GridSize.Y) * row) + col;
		return Index;
	}	
	return Index;
}

void APathFinderVolume::UpdateGridArrayByLocation(const FVector2D& Location, bool bCanMove)
{
	const int32 GridIdx = GetGridArrayIndexByLocation(Location);
	Grids[GridIdx].SetCanMove(bCanMove);
}

void APathFinderVolume::UpdateGridArrayByIndex(const int32& Index, const FPathFindGridInfo& UpdateGridInfo)
{
	Grids[Index] = UpdateGridInfo;
}
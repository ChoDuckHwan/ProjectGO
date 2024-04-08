// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/PathFind/PathFindGrid.h"
#include "Components/BoxComponent.h"

void FPathFindGridInfo::SetRow(const int32& Row)
{
	row = Row;
}

void FPathFindGridInfo::SetCow(const int32& Col)
{
	col = Col;
}

void FPathFindGridInfo::SetWorldLocation(const FVector2D& Location)
{
	WorldLocation = Location;
}

const int32& FPathFindGridInfo::GetRow() const
{
	return row;
}

const int32& FPathFindGridInfo::GetCol() const
{
	return col;
}

const bool& FPathFindGridInfo::GetCanMove() const
{
	return bCanMove;
}

const FVector2D& FPathFindGridInfo::GetWorldLocation() const
{
	return WorldLocation;
}

void FPathFindGridInfo::SetCanMove(const bool& InCanMove)
{
	bCanMove = InCanMove;
}

APathFindGrid::APathFindGrid(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

const bool& APathFindGrid::GetCanMove() const
{
	return GridInfo.GetCanMove();
}

void APathFindGrid::SetGridInfo(const FPathFindGridInfo& NewGridInfo)
{
	GridInfo = NewGridInfo;
}

const FPathFindGridInfo& APathFindGrid::GetGridInfo() const
{
	return GridInfo;
}

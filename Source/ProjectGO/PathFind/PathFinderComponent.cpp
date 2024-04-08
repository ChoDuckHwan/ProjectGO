// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/PathFind/PathFinderComponent.h"
#include "ProjectGO/Utilities/PriorityQueue.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectGO/GameState/GOGameState.h"
// Sets default values for this component's properties
UPathFinderComponent::UPathFinderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->ClearSplinePoints();
	DistanceToGoal = 0.0f;
	GoalDistanceThreshold = 25.f;
	DistanceThreshold = 25.f;
}


// Called when the game starts
void UPathFinderComponent::BeginPlay()
{
	Super::BeginPlay();
	if(GetWorld())
	{
		if(UNetDriver* NetDriver = GetWorld()->GetNetDriver())
		{
			if(!NetDriver->IsServer() && GetOwner())
			{
				const FVector ActorLocation(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y, 0);
				BeforeLastPoint = ActorLocation;
				UpdatePathFindGridCanMove(FVector2D(BeforeLastPoint.X, BeforeLastPoint.Y), false);
			}
		}		
	}
}

void UPathFinderComponent::Navigate(const TArray<FVector2D>& PathPoints)
{
	UpdatePathFindGridCanMove(FVector2D(BeforeLastPoint.X, BeforeLastPoint.Y), true);

	for (int SplineIndex = 0; SplineIndex < Spline->GetNumberOfSplinePoints(); ++SplineIndex)
	{
		const FVector BeforesLocation = Spline->GetWorldLocationAtSplinePoint(SplineIndex);
		UpdatePathFindGridCanMove(FVector2D(BeforesLocation.X, BeforesLocation.Y), true);
	}	
	Spline->ClearSplinePoints();
	for (const FVector2D& Point : PathPoints)
	{
		FVector Point_3D(Point.X, Point.Y, 0);
		Spline->AddSplineWorldPoint(Point_3D);
		Goal = Point_3D;
	}
	
	Length = Spline->GetSplineLength();
	Spline->Duration = Length;
	CurrentIndex = 1;
	NextPoint = Spline->GetWorldLocationAtSplinePoint(CurrentIndex);
}

void UPathFinderComponent::Stop() const
{
	Spline->ClearSplinePoints();
}

void UPathFinderComponent::UpdatePathFindGridCanMove(const FVector2D& Location, bool bCanMove)
{	
	if (GetWorld())
	{
		if (AGOGameState* GameState = Cast<AGOGameState>(UGameplayStatics::GetGameState(GetWorld())))
		{
			GameState->UpdatePathFindGridCanMoveByLocation(Location, bCanMove);
		}
	}
}

// Called every frame
void UPathFinderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(Spline->GetNumberOfSplinePoints() == 0)
	{
		return;
	}

	const FVector ActorLocation(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y, 0);

	DistanceToGoal = (Goal - ActorLocation).Size2D();
	if (DistanceToGoal <= GoalDistanceThreshold)
	{
		const FVector NowPoint = Spline->GetWorldLocationAtSplinePoint(CurrentIndex);
		BeforeLastPoint = NowPoint;
		UpdatePathFindGridCanMove(FVector2D(NowPoint.X, NowPoint.Y), false);

		for(int SplineIndex = 0; SplineIndex < Spline->GetNumberOfSplinePoints() - 1; ++SplineIndex)
		{
			const FVector BeforesLocation = Spline->GetWorldLocationAtSplinePoint(SplineIndex);
			UpdatePathFindGridCanMove(FVector2D(BeforesLocation.X, BeforesLocation.Y), true);
		}

		Spline->ClearSplinePoints();
		return;
	}

	const float DistanceToNextPoint = (NextPoint - ActorLocation).Size2D();
	if (DistanceToNextPoint <= DistanceThreshold)
	{
		CurrentIndex += 1;
		NextPoint = Spline->GetWorldLocationAtSplinePoint(CurrentIndex);
	}
	
	const FVector Direction = (NextPoint - ActorLocation).GetSafeNormal2D();	
	const FVector ActorDirection = GetOwner()->GetActorRotation().Vector().GetSafeNormal2D();	
	Cast<APawn>(GetOwner())->AddMovementInput(Direction, 1.0f);
}

const TArray<FVector2D> UPathFinderComponent::FindPath(const TArray<FPathFindGridInfo>& GridArray, const int32& StartGridIndex, const int32& TargetGridIndex, const int32& ColSize, const FVector2D& FinalLocation)
{
	if (GridArray.IsEmpty() || GridArray.Num() <= StartGridIndex || GridArray.Num() <= TargetGridIndex)
	{
		return TArray<FVector2D>();
	}
	const int CrossCost = 10;
	const int DiagonalCost = 14;
	int32 TargetGridIndexRow = TargetGridIndex / ColSize;
	int32 TargetGridIndexCol = TargetGridIndex % ColSize;

	TMap<int32, FAStarPathFindInfo> CloseList;
	TPriorityQueue<FAStarPathFindInfo> PQ_OpenList;
	TMap<int32, FAStarPathFindInfo> PQ_OpenListMap;
	
	FAStarPathFindInfo StartGrid;
	StartGrid.row = StartGridIndex / ColSize;
	StartGrid.col = StartGridIndex % ColSize;
	StartGrid.GCost = 0;
	StartGrid.FCost = 0;
	StartGrid.HCost = 0;
	StartGrid.WorldLocation = GridArray[StartGridIndex].GetWorldLocation();
	for (const auto AroundGrid : GetAroundGrids(GridArray, GridArray[StartGridIndex].GetRow(), GridArray[StartGridIndex].GetCol(), GridArray.Num()/ColSize, ColSize))
	{
		FAStarPathFindInfo ArroundGridInfo;
		ArroundGridInfo.WorldLocation = AroundGrid.GetWorldLocation();
		ArroundGridInfo.row = AroundGrid.GetRow();
		ArroundGridInfo.col = AroundGrid.GetCol();
		ArroundGridInfo.ParentInfo = MakeShared<FAStarPathFindInfo>(StartGrid);

		const bool Cross = StartGrid.row == ArroundGridInfo.row || StartGrid.col == ArroundGridInfo.col;
		const int PlusCost = Cross ? CrossCost : DiagonalCost;
		ArroundGridInfo.GCost = PlusCost;

		ArroundGridInfo.HCost = (FMath::Abs(ArroundGridInfo.row - TargetGridIndexRow) + FMath::Abs(ArroundGridInfo.col - TargetGridIndexCol)) * 10;
		ArroundGridInfo.FCost = ArroundGridInfo.GCost + ArroundGridInfo.HCost;
		PQ_OpenListMap.Add(ArroundGridInfo.row * ColSize + ArroundGridInfo.col, ArroundGridInfo);
		PQ_OpenList.Push(ArroundGridInfo, ArroundGridInfo.FCost);
	}

	CloseList.Add(StartGridIndex, StartGrid);
	while (!PQ_OpenList.IsEmpty())
	{
		FAStarPathFindInfo NowGrid;
		NowGrid = PQ_OpenList.Pop();

		const int32 NowGridIndex = NowGrid.row * ColSize + NowGrid.col;
		CloseList.Add(NowGridIndex, NowGrid);
		PQ_OpenListMap.Remove(NowGridIndex);

		if (CloseList.Find(TargetGridIndex))
		{
			//UE_LOG(LogTemp, Log, TEXT("Target Area Added"));
			break;
		}

		for (const auto AroundGrid : GetAroundGrids(GridArray, GridArray[NowGridIndex].GetRow(), GridArray[NowGridIndex].GetCol(), GridArray.Num() / ColSize, ColSize))
		{
			FAStarPathFindInfo AroundGridInfo;
			AroundGridInfo.WorldLocation = AroundGrid.GetWorldLocation();
			AroundGridInfo.row = AroundGrid.GetRow();
			AroundGridInfo.col = AroundGrid.GetCol();
			const int32 ArroundGridIndex = AroundGridInfo.row * ColSize + AroundGridInfo.col;
			if (CloseList.Find(ArroundGridIndex))
			{
				continue;
			}
			else
			{
				const bool Cross = NowGrid.row == AroundGridInfo.row || NowGrid.col == AroundGridInfo.col;
				const int PlusCost = Cross ? CrossCost : DiagonalCost;
				if (FAStarPathFindInfo* FindedGrid = PQ_OpenListMap.Find(ArroundGridIndex))
				{
					if((NowGrid.GCost + PlusCost) < AroundGridInfo.GCost)
					{
						AroundGridInfo.ParentInfo = MakeShared<FAStarPathFindInfo>(NowGrid);
						FindedGrid->GCost = NowGrid.GCost + PlusCost;
						FindedGrid->FCost = FindedGrid->GCost + FindedGrid->HCost;
					}
				}
				else
				{
					AroundGridInfo.GCost = NowGrid.GCost + PlusCost;
					AroundGridInfo.HCost = (FMath::Abs(AroundGridInfo.row - TargetGridIndexRow) + FMath::Abs(AroundGridInfo.col - TargetGridIndexCol)) * 10;
					AroundGridInfo.FCost = AroundGridInfo.GCost + AroundGridInfo.HCost;
					AroundGridInfo.ParentInfo = MakeShared<FAStarPathFindInfo>(NowGrid);

					PQ_OpenList.Push(AroundGridInfo, AroundGridInfo.FCost);
					PQ_OpenListMap.Add(AroundGridInfo.row * ColSize + AroundGridInfo.col, AroundGridInfo);
				}
			}
		}
	}
	return MakePathFromClosedList(CloseList, GridArray, TargetGridIndex, FinalLocation, ColSize);
}

TArray<FVector2D> UPathFinderComponent::MakePathFromClosedList(const TMap<int32, FAStarPathFindInfo>& CloseList, const TArray<FPathFindGridInfo>& GridArray, const int& TargetGridIndex, const FVector2D& FinalLocation, const int& ColSize)
{
	TArray<FVector2D> Path;
	Path.Empty();
	FAStarPathFindInfo TargetClosedGrid;

	if (const FAStarPathFindInfo* CloseGrid = CloseList.Find(TargetGridIndex))
	{
		TargetClosedGrid = *CloseGrid;
		Path.Insert(FinalLocation, 0);
	}
	else
	{		
		float MinDistance = TNumericLimits<float>::Max();
		float Distance_TargetGrid = 0.0f;
		for (const auto& ClosedGrid : CloseList)
		{
			Distance_TargetGrid = FVector2D::Distance(GridArray[TargetGridIndex].GetWorldLocation(), ClosedGrid.Value.WorldLocation);
			if (MinDistance > Distance_TargetGrid)
			{
				MinDistance = Distance_TargetGrid;
				TargetClosedGrid = ClosedGrid.Value;
			}
		}
		FVector TraceStartLocation = FVector(TargetClosedGrid.WorldLocation.X, TargetClosedGrid.WorldLocation.Y, GetOwner()->GetActorLocation().Z);
		FVector TraceEndLocation = FVector(FinalLocation.X, FinalLocation.Y, GetOwner()->GetActorLocation().Z);
		
		FHitResult HitResult;
		if(UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), TraceStartLocation, TraceEndLocation, 25.f, 25.f, TraceTypeQuery1, false, TArray<AActor*>(), EDrawDebugTrace::None, HitResult, true))
		{
			Path.Insert(FVector2D(HitResult.Location.X, HitResult.Location.Y), 0);
		}		
	}

	TSharedPtr<FAStarPathFindInfo> ParentGrid = TargetClosedGrid.ParentInfo;
	while (ParentGrid.IsValid())
	{
		Path.Insert(ParentGrid.Get()->WorldLocation, 0);
		ParentGrid = ParentGrid->ParentInfo;
	}

	return Path;
}


const TArray<FPathFindGridInfo> UPathFinderComponent::GetAroundGrids(const TArray<FPathFindGridInfo>& GridArray, const int& Row_NowGrid, const int& Col_NowGrid, const int32& RowSize, const int32& ColSize)
{

	TArray<FPathFindGridInfo> ArroundGrids;
	const TArray<int32> Arr_Search_Row = { -1, -1, -1, 0, 0, 1, 1, 1 };
	const TArray<int32> Arr_Search_Col = { -1, 0, 1, -1, 1, -1, 0, 1 };

	int SearchRow = 0;
	int SearchCol = 0;

	for (int i = 0; i < Arr_Search_Row.Num(); ++i)
	{
		SearchRow = Row_NowGrid + Arr_Search_Row[i];
		SearchCol = Col_NowGrid + Arr_Search_Col[i];
		if (SearchRow >= 0 && SearchRow < RowSize && SearchCol >= 0 && SearchCol < ColSize)
		{
			const int32 SearchGridIndex = SearchRow * ColSize + SearchCol;
			if (GridArray[SearchGridIndex].GetCanMove())
			{
				ArroundGrids.Add(GridArray[SearchGridIndex]);
			}
		}
	}

	return ArroundGrids;
}
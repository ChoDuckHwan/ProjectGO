// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SplineComponent.h"
#include "ProjectGO/PathFind/PathFindGrid.h"
#include "PathFinderComponent.generated.h"

struct FAStarPathFindInfo
{
	int row = 0;
	int col = 0;

	int GCost = INT_MAX;
	int FCost = INT_MAX;
	int HCost = INT_MAX;

	FVector2D WorldLocation;

	TSharedPtr<FAStarPathFindInfo> ParentInfo;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTGO_API UPathFinderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPathFinderComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USplineComponent* Spline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Goal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector NextPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector BeforeLastPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DistanceToGoal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GoalDistanceThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceThreshold;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Length;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentIndex;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	const TArray<FVector2D> FindPath(const TArray<FPathFindGridInfo>& GridArray, const int32& StartGridIndex, const int32& TargetGridIndex, const int32& ColSize, const FVector2D& FinalLocation);

	UFUNCTION(BlueprintCallable)
	const TArray<FPathFindGridInfo> GetAroundGrids(const TArray<FPathFindGridInfo>& GridArray, const int& Row_NowGrid, const int& Col_NowGrid, const int32& RowSize, const int32& ColSize);

	UFUNCTION(BlueprintCallable)
	void Navigate(const TArray<FVector2D>& PathPoints);

	UFUNCTION(BlueprintCallable)
	void Stop()const;

	UFUNCTION()
	void UpdatePathFindGridCanMove(const FVector2D& Location, bool bCanMove);

	TArray<FVector2D> MakePathFromClosedList(const TMap<int32, FAStarPathFindInfo>& CloseList, const TArray<FPathFindGridInfo>& GridArray, const int& TargetGridIndex, const FVector2D& FinalLocation, const int& ColSize);
};

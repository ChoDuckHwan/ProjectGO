// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectGO/PathFind/PathFindGrid.h"
#include "PathFinderVolume.generated.h"

class  APathFindGrid;

UCLASS()
class PROJECTGO_API APathFinderVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathFinderVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grid)
	FVector2D MapSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Grid)
	FVector2D GridSize;

private:
	UPROPERTY()
	TArray<FPathFindGridInfo> Grids;
		
	TArray<int16> ChangeGridIdxList;

public:
	/*UFUNCTION()
	void OnRep_ChangeGridIdxList();*/

	UFUNCTION(BlueprintCallable)
	void AddGrid(const FPathFindGridInfo& NewGrid);

	UFUNCTION(BlueprintCallable)
	void SetGrids(const TArray<FPathFindGridInfo>& NewGrids);

	UFUNCTION(BlueprintCallable)
	void ResetGrid();

	UFUNCTION(BlueprintCallable)
	const TArray<FPathFindGridInfo>&  GetGrids() const;
	
	UFUNCTION(BlueprintCallable)
	int32 GetGridArrayIndexByLocation(const FVector2D& Location) const;

	UFUNCTION(BlueprintCallable)
	void UpdateGridArrayByLocation(const FVector2D& Location, bool bCanMove);

	UFUNCTION(BlueprintCallable)
	void UpdateGridArrayByIndex(const int32& Index, const FPathFindGridInfo& UpdateGridInfo);
	

};

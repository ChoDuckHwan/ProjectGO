// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathFindGrid.generated.h"

/**
 * 
 */

class UBoxComponent;

USTRUCT(Blueprintable)
struct FPathFindGridInfo
{
	
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 row = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 col = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D WorldLocation{0.f, 0.f};

public:
	void SetRow(const int32& Row);
	void SetCow(const int32& Col);
	void SetWorldLocation(const FVector2D& Location);

	const int32& GetRow() const;
	const int32& GetCol() const;
	const bool& GetCanMove() const;
	const FVector2D& GetWorldLocation() const;
	void SetCanMove(const bool& InCanMove);
};

UCLASS()
class PROJECTGO_API APathFindGrid : public AActor
{
	GENERATED_BODY()

private:
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPathFindGridInfo GridInfo;
	
public:
	APathFindGrid(const FObjectInitializer& ObjectInitializer);
	
	const bool& GetCanMove() const;

	UFUNCTION(BlueprintCallable)
	void SetGridInfo(const FPathFindGridInfo& NewGridInfo);

	UFUNCTION(BlueprintCallable)
	const FPathFindGridInfo& GetGridInfo() const;
};

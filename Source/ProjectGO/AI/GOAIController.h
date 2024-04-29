// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GOAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTree;
/**
 * 
 */
UCLASS()
class PROJECTGO_API AGOAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGOAIController();

protected:
	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BlackboardComponent;

	UPROPERTY()
	TObjectPtr<UBehaviorTree> BehaviorTree;
};

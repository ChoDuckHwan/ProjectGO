// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/AI/GOAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AGOAIController::AGOAIController()
{
	bWantsPlayerState = true;
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(BlackboardComponent);
	BehaviorTree = CreateDefaultSubobject<UBehaviorTree>("BehaviorTree");
	check(BehaviorTree);
}

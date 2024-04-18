// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/UI/GOWidgetController.h"


void UGOWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;
}

void UGOWidgetController::BroadcastInitValue()
{

}

void UGOWidgetController::BindCallbacksToDependencies()
{
}

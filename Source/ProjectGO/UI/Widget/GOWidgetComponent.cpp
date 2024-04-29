// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/UI/Widget/GOWidgetComponent.h"

void UGOWidgetComponent::SetWidget(UUserWidget* InWidget)
{
	Super::SetWidget(InWidget);
}

void UGOWidgetComponent::UpdateWidget()
{
	Super::UpdateWidget();

	if(GetUserWidgetObject())
	{
		WidgetCreateFinished.Broadcast(GetUserWidgetObject());		
	}
}

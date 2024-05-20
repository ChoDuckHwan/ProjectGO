// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/UI/GOUserWidgetBase.h"

void UGOUserWidgetBase::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

void UGOUserWidgetBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	K2_WidgetPropertyChanged();
}

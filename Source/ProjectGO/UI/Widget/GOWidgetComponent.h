// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GOWidgetComponent.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FWidgetCreateFinishedSignature, UUserWidget*);

UCLASS(BlueprintType, Blueprintable)
class PROJECTGO_API UGOWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	virtual void SetWidget(UUserWidget* InWidget) override;

	virtual void UpdateWidget() override;
public:
	FWidgetCreateFinishedSignature WidgetCreateFinished;
};

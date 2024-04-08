// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/HUD/InGameHud.h"
#include "Blueprint/UserWidget.h"
#include "ProjectGO/UI/GOUserWidgetBase.h"
#include "ProjectGO/UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AInGameHud::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if(OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

void AInGameHud::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass uninitialized, please fill out BP_InGameHud"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass uninitialized, please fill out BP_InGameHud"));

	OverlayWidget = CreateWidget<UGOUserWidgetBase>(GetWorld(), OverlayWidgetClass);
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);	

	if (OverlayWidget)
	{
		UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
		OverlayWidget->SetWidgetController(WidgetController);
		OverlayWidget->AddToViewport();
		WidgetController->BroadcastInitValue();
	}	
}

void AInGameHud::BeginPlay()
{
	Super::BeginPlay();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/HUD/InGameHud.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "ProjectGO/UI/GOUserWidgetBase.h"
#include "ProjectGO/UI/WidgetController/GOAttributeWidgetController.h"
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

UGOAttributeWidgetController* AInGameHud::GetAttributeWidgetController(const FWidgetControllerParams& WCParams)
{
	if (GOAttributeWidgetController == nullptr)
	{
		GOAttributeWidgetController = NewObject<UGOAttributeWidgetController>(this, AttributeWidgetControllerClass);
		GOAttributeWidgetController->SetWidgetControllerParams(WCParams);
		GOAttributeWidgetController->BindCallbacksToDependencies();
	}
	return GOAttributeWidgetController;
}

void AInGameHud::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass uninitialized, please fill out BP_InGameHud"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass uninitialized, please fill out BP_InGameHud"));

	OverlayWidget = CreateWidget<UGOUserWidgetBase>(GetWorld(), OverlayWidgetClass);
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);	
	//UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(GetOwningPlayerController(), OverlayWidget);
	
	if (OverlayWidget)
	{
		UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
		OverlayWidget->SetWidgetController(WidgetController);
		OverlayWidget->AddToViewport();
		WidgetController->BroadcastInitValue();
	}	
}

void AInGameHud::InitAttribute(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(AttributeWidgetClass, TEXT("AttributeWidgetClass uninitialized, please fill out BP_InGameHud"));
	checkf(AttributeWidgetControllerClass, TEXT("AttributeWidgetControllerClass uninitialized, please fill out BP_InGameHud"));

	AttributeWidget = CreateWidget<UGOUserWidgetBase>(GetWorld(), AttributeWidgetClass);
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	//UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(GetOwningPlayerController(), OverlayWidget);

	if (AttributeWidget)
	{
		UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
		AttributeWidget->SetWidgetController(WidgetController);
		AttributeWidget->AddToViewport();
		WidgetController->BroadcastInitValue();
	}
}

void AInGameHud::BeginPlay()
{
	Super::BeginPlay();
}

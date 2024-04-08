// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectGO/HUD/GOHUDBase.h"
#include "InGameHud.generated.h"

/**
 * 
 */
class UGOUserWidgetBase;
class UOverlayWidgetController;
struct FWidgetControllerParams;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class PROJECTGO_API AInGameHud : public AGOHUDBase
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<UGOUserWidgetBase> OverlayWidget;
	
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGOUserWidgetBase> OverlayWidgetClass;

	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

};

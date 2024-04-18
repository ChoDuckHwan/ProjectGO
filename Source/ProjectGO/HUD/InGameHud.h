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
class UGOAttributeWidgetController;

UCLASS()
class PROJECTGO_API AInGameHud : public AGOHUDBase
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<UGOUserWidgetBase> OverlayWidget;

	UPROPERTY()
	TObjectPtr<UGOUserWidgetBase> AttributeWidget;

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	UGOAttributeWidgetController* GetAttributeWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	void InitAttribute(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);


protected:
	virtual void BeginPlay() override;

private:
	//Overlay Start
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGOUserWidgetBase> OverlayWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	//Overlay End

	//AttributeStart
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGOUserWidgetBase> AttributeWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGOAttributeWidgetController> AttributeWidgetControllerClass;

	TObjectPtr<UGOAttributeWidgetController> GOAttributeWidgetController;

	//Attribute End
	

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GOAbilityBFL.generated.h"

class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class PROJECTGO_API UGOAbilityBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|WidgetController")
	static UGOAttributeWidgetController* GetAttributeWidgetController(const UObject* WorldContextObject);
};

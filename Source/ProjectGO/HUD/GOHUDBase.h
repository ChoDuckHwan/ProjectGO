// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOHudInterface.h"
#include "GameFramework/HUD.h"
#include "GOHUDBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGO_API AGOHUDBase : public AHUD, public IGOHudInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void BP_OnChangeMouseCursorType_Implementation(TSubclassOf<UGOMouseCursorWidget> MouseCursorType) override;

};

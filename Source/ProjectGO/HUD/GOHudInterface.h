// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GOHudInterface.generated.h"

class UGOMouseCursorWidget;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGOHudInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTGO_API IGOHudInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	void BP_OnChangeMouseCursorType(TSubclassOf<UGOMouseCursorWidget> MouseCursorType);
	virtual void BP_OnChangeMouseCursorType_Implementation(TSubclassOf<UGOMouseCursorWidget> MouseCursorType);

};

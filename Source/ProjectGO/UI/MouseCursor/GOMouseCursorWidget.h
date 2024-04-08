// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GOMouseCursorWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGO_API UGOMouseCursorWidget : public UUserWidget
{
	GENERATED_BODY()
	
};

UCLASS()
class PROJECTGO_API UGODefaultCursorType : public UGOMouseCursorWidget
{
	GENERATED_BODY()

};

UCLASS()
class PROJECTGO_API UGOAttackCursorType : public UGOMouseCursorWidget
{
	GENERATED_BODY()

};

UCLASS()
class PROJECTGO_API UGOAimCursorType : public UGOMouseCursorWidget
{
	GENERATED_BODY()

};
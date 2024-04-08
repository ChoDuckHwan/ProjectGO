// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectGO/UI/GOUserWidgetBase.h"
#include "UW_CharacterInfoBar.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGO_API UUW_CharacterInfoBar : public UGOUserWidgetBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;

public:
	void SetOwnerCharacter(AActor* NewOwnerActor);

	UFUNCTION(BlueprintCallable)
	const AActor* GetOwnerActor() const;
};

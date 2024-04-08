// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GOPlayerState.generated.h"

/**
 * 
 */

UCLASS()
class PROJECTGO_API AGOPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AGOPlayerState();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UGOAttributeSetBase* GetAttributeSetBase() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<class UGOAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<class UGOAttributeSetBase> AttributeSetBase;
};

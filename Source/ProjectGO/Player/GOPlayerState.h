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
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UGOAttributeSetBase* GetAttributeSetBase() const;

	int32 GetPlayerLevel()const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UGOAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UGOAttributeSetBase> AttributeSetBase;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UFUNCTION()
	void OnRep_Level(const int32& oldLevel);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOGameplayAbilityBase.h"
#include "GOProjectileSpell.generated.h"

class AGOProjectile;
/**
 * 
 */
UCLASS()
class PROJECTGO_API UGOProjectileSpell : public UGOGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AGOProjectile> ProjectileClass;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectGO/Character/Abilities/GOCharacterGameplayAbility.h"
#include "GA_NormalAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGO_API UGA_NormalAttack : public UGOCharacterGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int AttackCount = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<AActor> Projectile;
};

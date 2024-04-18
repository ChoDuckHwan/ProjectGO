// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ProjectGO/Enums/Damage.h"
#include "GOGameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGO_API UGOGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGOGameplayAbilityBase();

	const FGameplayTag& GetAbilityInputTag() const;
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, category = "Input")
	FGameplayTag AbilityInputTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "Ability")
	EGOAbilityID AbilityID = EGOAbilityID::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Ability")
	bool ActivateAbilityOnGranted = false;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange_Async.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);

/**
 * 
 */
UCLASS(BlueprintType, meta=(ExposedAsyncProxy = "AsyncTask"))
class PROJECTGO_API UWaitCooldownChange_Async : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;

	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly ="true"))
	static UWaitCooldownChange_Async* WaitForCooldownChange(UAbilitySystemComponent* ASC, const FGameplayTag& InCooldownTag);

	UFUNCTION(BlueprintCallable)
	void Endtask();

protected:	
	TWeakObjectPtr<UAbilitySystemComponent> ASC;
	
	FGameplayTag CooldownTag;

	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);

	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle EffectHandleActive);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectGO/UI/GOWidgetController.h"
#include "GOAttributeWidgetController.generated.h"
class UDA_AttributeInfo;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FGOAttributeInfo&, Info);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTGO_API UGOAttributeWidgetController : public UGOWidgetController
{
	GENERATED_BODY()

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;

public:
	virtual void BroadcastInitValue() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnPlayerStatChangedSignature AttributePointsChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnPlayerStatChangedSignature SpellPointsChangedDelegate;

	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(BlueprintCallable)
	void UpgradeSpell(const FGameplayTag& AttributeTag);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDA_AttributeInfo> AttributeInfo;
};

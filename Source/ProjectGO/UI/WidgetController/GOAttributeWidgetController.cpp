// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/UI/WidgetController/GOAttributeWidgetController.h"
#include "GameplayTagsManager.h"
#include "ProjectGO/Character/Abilities/AttributeSet/GOAttributeSetBase.h"
#include "ProjectGO/Character/Abilities/Data/DA_AttributeInfo.h"


void UGOAttributeWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,	const FGameplayAttribute& Attribute) const
{
	FGOAttributeInfo Info = AttributeInfo->FindAttributeInfoFroTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet.Get());
	AttributeInfoDelegate.Broadcast(Info);
}

void UGOAttributeWidgetController::BroadcastInitValue()
{
	UGOAttributeSetBase* AS = CastChecked<UGOAttributeSetBase>(AttributeSet);
	check(AttributeInfo);
	
	for(const auto& Attribute : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Attribute.Key, Attribute.Value());		
	}
}

void UGOAttributeWidgetController::BindCallbacksToDependencies()
{
	UGOAttributeSetBase* AS = CastChecked<UGOAttributeSetBase>(AttributeSet);
	check(AttributeInfo);
	for(const auto& Attribute : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute.Value()).AddLambda([&, Attribute](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Attribute.Key, Attribute.Value());
			});
	}	
}

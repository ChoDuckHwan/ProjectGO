// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectGO/GOGameplayTags.h"
#include "DA_AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FGOAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDesc = FText();

	UPROPERTY(BlueprintReadOnly)	/* BlueprintReadOnly option is not override this value from blueprint dataasset*/
	float AttributeValue = 0.f;
};

/**
 * 
 */
UCLASS()
class PROJECTGO_API UDA_AttributeInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	FGOAttributeInfo FindAttributeInfoFroTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;

	const TArray<FGOAttributeInfo>& GetAttributeInfos();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGOAttributeInfo> AttributeInfos;
};

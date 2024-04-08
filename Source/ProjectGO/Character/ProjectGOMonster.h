// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOPlayerCharacter.h"
#include "ProjectGO/Character/ProjectGOCharacter.h"
#include "ProjectGOMonster.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGO_API AProjectGOMonster : public AProjectGOCharacter
{
	GENERATED_BODY()


protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UGOAbilitySystemComponent> MonsterAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UGOAttributeSetBase> MonsterAttributeSetBase;

public:
	AProjectGOMonster(const class FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void InitializeAbilityValue(AGOPlayerState* PS) override;

};

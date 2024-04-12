// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectGO/Character/ProjectGOCharacter.h"
#include "ProjectGO/Interaction/MonsterInterface.h"
#include "ProjectGOMonster.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGO_API AProjectGOMonster : public AProjectGOCharacter, public IMonsterInterface
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Character Class Defaults", ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UGOAbilitySystemComponent> MonsterAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UGOAttributeSetBase> MonsterAttributeSetBase;

	virtual void InitializeAbilityValue(AGOPlayerState* PS) override;

	UFUNCTION()
	virtual void OnRep_Level(const int32& oldLevel);
public:
	AProjectGOMonster(const class FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	/* Combat Interface*/
	virtual int32 GetLevel() override;
	/* Combat Interface*/
	
};

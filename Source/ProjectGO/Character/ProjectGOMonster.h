// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectGO/Character/ProjectGOCharacter.h"
#include "ProjectGO/Interaction/MonsterInterface.h"
#include "ProjectGO/Character/Abilities/Data/CharacterClassInfo.h"
#include "ProjectGOMonster.generated.h"

class UBehaviorTree;
class AGOAIController;
/**
 * 
 */
UCLASS()
class PROJECTGO_API AProjectGOMonster : public AProjectGOCharacter, public IMonsterInterface
{
	GENERATED_BODY()

private:

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<class UGOAbilitySystemComponent> MonsterAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<class UGOAttributeSetBase> MonsterAttributeSetBase;

	virtual void InitializeAbilityValue(AGOPlayerState* PS) override;
	virtual void InitializeAttributes() const override;

	UFUNCTION()
	virtual void OnRep_Level(const int32& oldLevel);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults", ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass MonsterClass = ECharacterClass::Warrior;

	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY()
	TWeakObjectPtr<AGOAIController> GOAIController;

	UPROPERTY(BlueprintReadWrite, Category="Combat")
	TWeakObjectPtr<AActor> CombatTarget;	
public:
	AProjectGOMonster(const class FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PossessedBy(AController* NewController) override;
	
	/* Combat Interface*/
	virtual int32 GetLevel() const override;

	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual  const AActor* GetCombatTarget_Implementation() const override;
	/* Combat Interface*/

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	virtual void Die() override;
};

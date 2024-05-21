// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "ProjectGO/Character/Abilities/Data/LevelupInfo.h"
#include "GOPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, const int32& /*Stat Value*/)

/**
 * 
 */

UCLASS()
class PROJECTGO_API AGOPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AGOPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UGOAttributeSetBase* GetAttributeSetBase() const;
	UAttributeSet* GetAttributeSet() const;
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetPlayerXP() const { return XP; }

	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelupInfo> LevelupInfo;
	
	void AddXP(const int32& PlusXP);
	void AddToLevel(const int32& PlusLevel);

	void AddToAttributePoints(const int32& AddValue);
	void AddToSpellPoints(const int32& AddValue);
	
	void SetXP(const int32& NewXP);
	void SetLevel(const int32& NewLevel);

	FOnPlayerStatChanged LevelChangedDelegate;	
	FOnPlayerStatChanged XPChangedDelegate;
	FOnPlayerStatChanged AttributePointChangedDelegate;
	FOnPlayerStatChanged SpellPointChangedDelegate;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UGOAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UGOAttributeSetBase> AttributeSetBase;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SpellPoints)
	int32 SpellPoints = 0;
	
	UFUNCTION()
	void OnRep_Level(const int32& oldLevel);

	UFUNCTION()
	void OnRep_XP(const int32& oldXP);

	UFUNCTION()
	void OnRep_AttributePoints(const int32& OldValue);
	UFUNCTION()
	void OnRep_SpellPoints(const int32& OldValue);
};

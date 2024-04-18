// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "ProjectGO/Interaction/CombatInterface.h"
#include "ProjectGOCharacter.generated.h"

class UGOAbilitySystemComponent;
class UGOAttributeSetBase;
class UWidgetComponent;
class AGOPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AProjectGOCharacter*, Character);

UCLASS(Blueprintable)
class AProjectGOCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	AProjectGOCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(BlueprintAssignable, category="Character")
	FCharacterDiedDelegate OnCharacterDied;

	UFUNCTION(BlueprintCallable, category = "Character")
	virtual bool IsAlive() const;

	UFUNCTION(BlueprintCallable, category = "Character")
	virtual int32 GetAbilityLevel(EGOAbilityID AbilityID) const;

	virtual void RemoveCharacterAbilities();

	virtual void Die();

	UFUNCTION(BlueprintCallable, category = "Character")
	virtual void FinishDying();	

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;	
		
	virtual void OnRep_PlayerState() override;

	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:	

	virtual void InitializeAbilityValue(AGOPlayerState* PS);

	//SocketName that fired character's projectile location
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName WeaponTipSocketName;

	virtual FVector GetCombatSocketLocation() override;

	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<class UGOAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<class UGOAttributeSetBase> AttributeSetBase;
	
	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeadTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category ="Character")
	FText CharacterName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "Animation")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "Animation")
	TArray<TSubclassOf<class UGameplayAbility>> CharacterAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultSeceondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultVitalAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	virtual void AddCharacterAbilities();
	virtual void InitializeAttributes() const;
	virtual void AddStartupEffects();

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float SelfLevel) const;
		
	UFUNCTION(BlueprintCallable)
	virtual UGOAttributeSetBase* GetAttributeSetBase() const;

	void BindAttributes();	
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
		
	virtual void BindMouseCursorEvent();

	UFUNCTION(BlueprintImplementableEvent) void CharacterOnClicked(AActor* TouchedActor, FKey ButtonPressed);
	UFUNCTION(BlueprintImplementableEvent) void CharacterCursorBeginOvered(AActor* TouchedActor);
	UFUNCTION(BlueprintImplementableEvent) void CharacterCursorEndOvered(AActor* TouchedActor);


};


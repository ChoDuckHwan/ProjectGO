// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "ProjectGOCharacter.generated.h"

class UGOAbilitySystemComponent;
class UGOAttributeSetBase;
class UWidgetComponent;
class AGOPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AProjectGOCharacter*, Character);

UCLASS(Blueprintable)
class AProjectGOCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
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

	UFUNCTION(BlueprintCallable, category="character|Attribute")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, category = "character|Attribute")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, category = "character|Attribute")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, category = "character|Attribute")
	float GetMaxMana() const;
	UFUNCTION(BlueprintCallable, category = "character|Attribute")
	int32 GetCharacterLevel() const;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;	
		
	virtual void OnRep_PlayerState() override;

	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void InitializeAbilityValue(AGOPlayerState* PS);

protected:	

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
	TArray<TSubclassOf<class UGOCharacterGameplayAbility>> CharacterAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	virtual void AddCharacterAbilities();
	virtual void InitializeAttributes();
	virtual void AddStartupEffects();

	virtual void SetHealth(float Health);
	virtual void SetMana(float Mana);
	virtual void SetMaxHealth(float MaxHealth);
	virtual void SetMaxMana(float MaxMana);

	FDelegateHandle HealthChangeDelegateHandle;
	FDelegateHandle MaxHealthChangeDelegateHandle;
	FDelegateHandle ManaChangeDelegateHandle;
	FDelegateHandle MaxManaChangeDelegateHandle;
	FDelegateHandle CharacterLevelDelegateHandle;

	UFUNCTION(BlueprintCallable)
	virtual UGOAttributeSetBase* GetAttributeSetBase() const;

	void BindAttributes();

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void ManaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxManaChanged(const FOnAttributeChangeData& Data);
	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(const float& newValue, const float& oldValue);
	UFUNCTION(BlueprintImplementableEvent)
	void OnMaxHealthChanged(const float& newValue, const float& oldValue);
	UFUNCTION(BlueprintImplementableEvent)
	void OnManaChanged(const float& newValue, const float& oldValue);
	UFUNCTION(BlueprintImplementableEvent)
	void OnMaxManaChanged(const float& newValue, const float& oldValue);
	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterLevelChanged(const float& newValue, const float& oldValue);

	virtual void BindMouseCursorEvent();

	UFUNCTION(BlueprintImplementableEvent) void CharacterOnClicked(AActor* TouchedActor, FKey ButtonPressed);
	UFUNCTION(BlueprintImplementableEvent) void CharacterCursorBeginOvered(AActor* TouchedActor);
	UFUNCTION(BlueprintImplementableEvent) void CharacterCursorEndOvered(AActor* TouchedActor);


};


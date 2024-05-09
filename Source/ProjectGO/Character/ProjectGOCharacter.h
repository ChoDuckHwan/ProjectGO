// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "ProjectGO/Interaction/CombatInterface.h"
#include "ProjectGO/UI/WidgetController/OverlayWidgetController.h"
#include "ProjectGOCharacter.generated.h"

class UGOAbilitySystemComponent;
class UGOAttributeSetBase;
class AGOPlayerState;
class UGOWidgetComponent;
class UNiagaraSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AProjectGOCharacter*, Character);

UCLASS(Blueprintable, BlueprintType)
class AProjectGOCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();
	virtual void MulticastHandleDeath_Implementation();
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	AProjectGOCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(BlueprintAssignable, category="Character")
	FCharacterDiedDelegate OnCharacterDied;

	UFUNCTION(BlueprintCallable, category = "Character")
	virtual bool IsAlive() const;

	UFUNCTION(BlueprintCallable, category = "Character")
	virtual int32 GetAbilityLevel(EGOAbilityID AbilityID) const;

	virtual void RemoveCharacterAbilities();

	virtual void Die() override;

	UFUNCTION(BlueprintCallable, category = "Character")
	virtual void FinishDying();	

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;	
		
	virtual void OnRep_PlayerState() override;

	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() const override;

	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> AttackMontages;

	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	
protected:
	UFUNCTION(BlueprintCallable)
	void SetWeaponMesh(USkeletalMeshComponent* WeaponMeshComponent);
	
	UPROPERTY(EditAnywhere)
	float RagdollLifeTime = 5.f;

	virtual void InitializeAbilityValue(AGOPlayerState* PS);

	//SocketName that fired character's projectile location
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName WeaponTipSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName RightHandSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName LeftHandSocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> BloodEffect;
	
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;

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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "UI|WidgetComponent")
	TObjectPtr<UGOWidgetComponent> HealthBar_CharacterHead;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "UI|HeadInfo")
	TSubclassOf<UUserWidget> MyHealthBar_HeadClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "UI|HeadInfo")
	TSubclassOf<UUserWidget> MyTeamHealthBar_HeadClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "UI|HeadInfo")
	TSubclassOf<UUserWidget> EnemyHealthBar_HeadClass;

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

//Head Healthbar delegate
protected:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature HealthChangedSignature;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature MaxHealthChangedSignature;
};


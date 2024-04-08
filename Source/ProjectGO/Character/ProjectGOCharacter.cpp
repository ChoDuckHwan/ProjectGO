// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectGOCharacter.h"

#include "Abilities/GOCharacterGameplayAbility.h"
#include "Abilities/AttributeSet/GOAttributeSetBase.h"
#include "UObject/ConstructorHelpers.h"
#include "ProjectGO/Character/Abilities/GOAbilitySystemComponent.h"
#include "ProjectGO/Enums/Damage.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"
#include "ProjectGO/HUD/InGameHud.h"
#include "ProjectGO/Player/GOPlayerState.h"
#include "ProjectGO/PlayerController/ProjectGOPlayerController.h"
#include "ProjectGO/UI/Character/UW_CharacterInfoBar.h"
//#include "ProjectGO/UI/Character/UW_CharacterHead.h"

AProjectGOCharacter::AProjectGOCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeadTag = FGameplayTag::RequestGameplayTag(FName("Effect.RemoveOnDeath"));	

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));

}


bool AProjectGOCharacter::IsAlive() const
{
	return GetHealth() > 0.f;
}

int32 AProjectGOCharacter::GetAbilityLevel(EGOAbilityID AbilityID) const
{
	return 1;
}

void AProjectGOCharacter::RemoveCharacterAbilities()
{
	if(GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->AbilitiesGiven)
	{
		return;
	}
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for(const FGameplayAbilitySpec& spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		/*if((spec.SourceObject == this) && CharacterAbilities.Contains(spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add((spec.Handle));
		}*/
	}

	for(int32 i=0; i< AbilitiesToRemove.Num(); ++i)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	Cast<UGOAbilitySystemComponent>(AbilitySystemComponent)->AbilitiesGiven = false;
}

void AProjectGOCharacter::Die()
{
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);

	if(AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAbilities();

		FGameplayTagContainer EffectTagToRemove;
		EffectTagToRemove.AddTag(EffectRemoveOnDeadTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagToRemove);
		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	if(DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		FinishDying();
	}
}

void AProjectGOCharacter::FinishDying()
{
	Destroy();
}

float AProjectGOCharacter::GetHealth() const
{
	if(GetAttributeSetBase())
	{
		return GetAttributeSetBase()->GetHealth();
	}
	return 0.0f;
}

float AProjectGOCharacter::GetMaxHealth() const
{
	if (GetAttributeSetBase())
	{
		return GetAttributeSetBase()->GetMaxHealth();
	}
	return 0.0f;
}

float AProjectGOCharacter::GetMana() const
{
	if (GetAttributeSetBase())
	{
		return GetAttributeSetBase()->GetMana();
	}
	return 0.0f;
}

float AProjectGOCharacter::GetMaxMana() const
{
	if (GetAttributeSetBase())
	{
		return GetAttributeSetBase()->GetMaxMana();
	}
	return 0.0f;
}

int32 AProjectGOCharacter::GetCharacterLevel() const
{
	if (GetAttributeSetBase())
	{
		return GetAttributeSetBase()->GetLevel();
	}
	return 0;
}

void AProjectGOCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AProjectGOCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();	
}

void AProjectGOCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(GetNetMode() != NM_DedicatedServer)
	{		
		BindMouseCursorEvent();
	}
}

UAbilitySystemComponent* AProjectGOCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void AProjectGOCharacter::InitializeAbilityValue(AGOPlayerState* PS)
{
	if (!AbilitySystemComponent.IsValid() || !AttributeSetBase.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent, AttributeSetBase Is Not Valid"));
	}
	BindAttributes();
	AbilitySystemComponent->SetTagMapCount(DeadTag, 0);
	InitializeAttributes();
	SetHealth(GetMaxHealth());
	SetMana(GetMaxMana());
	SetMaxHealth(GetMaxHealth());
	SetMaxMana(GetMaxMana());

	if(AProjectGOPlayerController* GOPlayerController = Cast<AProjectGOPlayerController>(GetController()))
	{
		if(AInGameHud* InGameHud = Cast<AInGameHud>(GOPlayerController->GetHUD()))
		{
			InGameHud->InitOverlay(GOPlayerController, GetPlayerState(), AbilitySystemComponent.Get(), AttributeSetBase.Get());			
		}
	}
}

void AProjectGOCharacter::AddCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->AbilitiesGiven)
	{
		return;
	}
	for(const TSubclassOf<UGOCharacterGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		if(!StartupAbility.GetDefaultObject())
		{
			continue;
		}
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID),
			static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityID)));
	}
}

void AProjectGOCharacter::InitializeAttributes()
{
	if(!AbilitySystemComponent.IsValid())
	{
		return;
	}
	if(!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
		
	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
	if(NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void AProjectGOCharacter::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || Cast<UGOAbilitySystemComponent>(AbilitySystemComponent)->StartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for(TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}
	Cast<UGOAbilitySystemComponent>(AbilitySystemComponent)->StartupEffectsApplied = true;
}

void AProjectGOCharacter::SetHealth(float Health)
{
	if(GetAttributeSetBase())
	{
		GetAttributeSetBase()->SetHealth(Health);
	}
}

void AProjectGOCharacter::SetMana(float Mana)
{
	if (GetAttributeSetBase())
	{
		GetAttributeSetBase()->SetMana(Mana);
	}
}

void AProjectGOCharacter::SetMaxHealth(float MaxHealth)
{
	if (GetAttributeSetBase())
	{
		GetAttributeSetBase()->SetMaxHealth(MaxHealth);
	}
}

void AProjectGOCharacter::SetMaxMana(float MaxMana)
{
	if (GetAttributeSetBase())
	{
		GetAttributeSetBase()->SetMaxMana(MaxMana);
	}
}

UGOAttributeSetBase* AProjectGOCharacter::GetAttributeSetBase() const
{
	return AttributeSetBase.Get();
}

void AProjectGOCharacter::BindAttributes()
{
	if (AbilitySystemComponent.IsValid() && AttributeSetBase.IsValid())
	{
		HealthChangeDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAttributeSetBase()->GetHealthAttribute()).AddUObject(this, &AProjectGOCharacter::HealthChanged);
		MaxHealthChangeDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAttributeSetBase()->GetMaxHealthAttribute()).AddUObject(this, &AProjectGOCharacter::MaxHealthChanged);
		ManaChangeDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAttributeSetBase()->GetManaAttribute()).AddUObject(this, &AProjectGOCharacter::ManaChanged);
		MaxManaChangeDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAttributeSetBase()->GetMaxManaAttribute()).AddUObject(this, &AProjectGOCharacter::MaxManaChanged);
		CharacterLevelDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAttributeSetBase()->GetLevelAttribute()).AddUObject(this, &AProjectGOCharacter::CharacterLevelChanged);
		
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AProjectGOCharacter::StunTagChanged);

		if (GetNetMode() == NM_Client)
		{
			OnHealthChanged(GetAttributeSetBase()->GetHealth(), GetAttributeSetBase()->GetHealth());
			OnMaxHealthChanged(GetAttributeSetBase()->GetMaxHealth(), GetAttributeSetBase()->GetMaxHealth());
			OnManaChanged(GetAttributeSetBase()->GetMana(), GetAttributeSetBase()->GetMana());
			OnMaxManaChanged(GetAttributeSetBase()->GetMaxMana(), GetAttributeSetBase()->GetMaxMana());
		}
	}
}

void AProjectGOCharacter::HealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("HealthChanged"));
	OnHealthChanged(Data.NewValue, Data.OldValue);
	if (GetNetMode() == NM_DedicatedServer)
	{
		UE_LOG(LogTemp, Log, TEXT("Server"));
	}
	if (GetNetMode() == NM_Client)
	{
		UE_LOG(LogTemp, Log, TEXT("Client"));
	}
}

void AProjectGOCharacter::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("MaxHealthChanged"));
	OnMaxHealthChanged(Data.NewValue, Data.OldValue);
}

void AProjectGOCharacter::ManaChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("ManaChanged"));
	OnManaChanged(Data.NewValue, Data.OldValue);
}

void AProjectGOCharacter::MaxManaChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("MaxManaChanged"));
	OnMaxManaChanged(Data.NewValue, Data.OldValue);
}

void AProjectGOCharacter::CharacterLevelChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("CharacterLevelChanged"));
	OnCharacterLevelChanged(Data.NewValue, Data.OldValue);
}

void AProjectGOCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}

void AProjectGOCharacter::BindMouseCursorEvent()
{
	OnClicked.AddDynamic(this, &AProjectGOCharacter::CharacterOnClicked);
	OnBeginCursorOver.AddDynamic(this, &AProjectGOCharacter::CharacterCursorBeginOvered);
	OnEndCursorOver.AddDynamic(this, &AProjectGOCharacter::CharacterCursorEndOvered);
}

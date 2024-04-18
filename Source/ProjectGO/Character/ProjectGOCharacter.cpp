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
#include "Net/UnrealNetwork.h"
#include "ProjectGO/HUD/InGameHud.h"
#include "ProjectGO/Player/GOPlayerState.h"
#include "ProjectGO/PlayerController/ProjectGOPlayerController.h"

//#include "ProjectGO/UI/Character/UW_CharacterHead.h"

void AProjectGOCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

AProjectGOCharacter::AProjectGOCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeadTag = FGameplayTag::RequestGameplayTag(FName("Effect.RemoveOnDeath"));
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}


bool AProjectGOCharacter::IsAlive() const
{
	return AttributeSetBase->GetHealth() > 0.f;
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
	AbilitySystemComponent->AbilityActorInfoSet();

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
	AbilitySystemComponent->AddCharacterAbilities(CharacterAbilities);
}

void AProjectGOCharacter::InitializeAttributes() const
{
	ApplyEffectToSelf(DefaultAttributes, 1.0f);
	ApplyEffectToSelf(DefaultSeceondaryAttributes, 1.0f);

	//In DefaultSeceondaryAttributes set vital max value so after call DefaultVitalAttributes apply
	ApplyEffectToSelf(DefaultVitalAttributes, 1.0f);	
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
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, ICombatInterface::GetLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}
	Cast<UGOAbilitySystemComponent>(AbilitySystemComponent)->StartupEffectsApplied = true;
}

void AProjectGOCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float SelfLevel) const
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}
	if (!GameplayEffectClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	const FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, SelfLevel, EffectContext);
	if (NewHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

UGOAttributeSetBase* AProjectGOCharacter::GetAttributeSetBase() const
{
	return AttributeSetBase.Get();
}

void AProjectGOCharacter::BindAttributes()
{
	
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

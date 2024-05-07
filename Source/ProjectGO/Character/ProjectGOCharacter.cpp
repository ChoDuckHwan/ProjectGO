// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectGOCharacter.h"

#include "Abilities/GOAbilityBFL.h"
#include "Abilities/GOCharacterGameplayAbility.h"
#include "Abilities/AttributeSet/GOAttributeSetBase.h"
#include "UObject/ConstructorHelpers.h"
#include "ProjectGO/Character/Abilities/GOAbilitySystemComponent.h"
#include "ProjectGO/Enums/Damage.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/Application/SlateApplication.h"
#include "ProjectGO/ProjectGO.h"
#include "ProjectGO/HUD/InGameHud.h"
#include "ProjectGO/Player/GOPlayerState.h"
#include "ProjectGO/PlayerController/ProjectGOPlayerController.h"
#include "ProjectGO/UI/GOUserWidgetBase.h"
#include "ProjectGO/UI/Widget/GOWidgetComponent.h"

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

	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);

	HealthBar_CharacterHead = CreateDefaultSubobject<UGOWidgetComponent>("HealthBar_Head");
	HealthBar_CharacterHead->SetupAttachment(GetRootComponent());
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
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->
		AbilitiesGiven)
	{
		return;
	}
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		/*if((spec.SourceObject == this) && CharacterAbilities.Contains(spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add((spec.Handle));
		}*/
	}

	for (int32 i = 0; i < AbilitiesToRemove.Num(); ++i)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	Cast<UGOAbilitySystemComponent>(AbilitySystemComponent)->AbilitiesGiven = false;
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
	if (GetNetMode() != NM_DedicatedServer)
	{
		BindMouseCursorEvent();
	}
}

UAbilitySystemComponent* AProjectGOCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

UAnimMontage* AProjectGOCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

bool AProjectGOCharacter::IsDead_Implementation() const
{
	return GetAttributeSetBase()->GetHealth() <= 0;
}

AActor* AProjectGOCharacter::GetAvatar_Implementation() const
{
	return GetAbilitySystemComponent()->GetAvatarActor();
}

TArray<FTaggedMontage> AProjectGOCharacter::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

void AProjectGOCharacter::InitializeAbilityValue(AGOPlayerState* PS)
{
	if (!AbilitySystemComponent.IsValid() || !AttributeSetBase.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("&&& AbilitySystemComponent, AttributeSetBase Is Not Valid"));
		return;
	}
	UGOAbilityBFL::GiveCommonAbilities(this, GetAbilitySystemComponent());
	if (!HasAuthority())
	{
		HealthBar_CharacterHead->WidgetCreateFinished.AddLambda([&](UUserWidget* CreatedWidget)
		{
			if (UGOUserWidgetBase* HeadWidget = Cast<UGOUserWidgetBase>(CreatedWidget))
			{
				HeadWidget->SetWidgetController(this);
				MaxHealthChangedSignature.Broadcast(AttributeSetBase.Get()->GetMaxHealth());
				HealthChangedSignature.Broadcast(AttributeSetBase.Get()->GetHealth());
			}
			//UE_LOG(LogTemp, Error, TEXT("&&& HeadWidget Name ::: %s , [Simulated ::: %s]"), *GetNameSafe(CreatedWidget),*GetNameSafe(this));
		});
		
		AbilitySystemComponent.Get()->GetGameplayAttributeValueChangeDelegate(AttributeSetBase.Get()->GetMaxHealthAttribute()).AddLambda([&](const FOnAttributeChangeData& Data)
		{
			MaxHealthChangedSignature.Broadcast(Data.NewValue);
		});
		AbilitySystemComponent.Get()->GetGameplayAttributeValueChangeDelegate(AttributeSetBase.Get()->GetHealthAttribute()).AddLambda([&](const FOnAttributeChangeData& Data)
		{
			HealthChangedSignature.Broadcast(Data.NewValue);
		});
		
		APlayerController* LPC = UGameplayStatics::GetPlayerController(this, 0);
		//After check team number, separate character health bar. 
		if (LPC)
		{
			if (LPC->PlayerState == nullptr || LPC->PlayerState != PS)
			{
				//Enemy Or MyTeamPlayerCharacter
				HealthBar_CharacterHead->SetWidgetClass(EnemyHealthBar_HeadClass);
			}
			if (LPC->PlayerState == PS && LPC->PlayerState)
			{
				//My
				HealthBar_CharacterHead->SetWidgetClass(MyHealthBar_HeadClass);
				if (AInGameHud* InGameHud = Cast<AInGameHud>(LPC->GetHUD()))
				{
					InGameHud->InitOverlay(LPC, GetPlayerState(), AbilitySystemComponent.Get(), AttributeSetBase.Get());
				}
			}
		}
	}
}

FVector AProjectGOCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	// Return correct socket based on montagetag.
	// you can make this TMap also.
	const FGOGameplayTags& GameplayTag = FGOGameplayTags::Get();
	if(MontageTag.MatchesTagExact(GameplayTag.Montage_Attack_Weapon))
	{
		return GetMesh()->GetSocketLocation(WeaponTipSocketName);
	}
	if(MontageTag.MatchesTagExact(GameplayTag.Montage_Attack_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if(MontageTag.MatchesTagExact(GameplayTag.Montage_Attack_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	return FVector();
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
	if (!HasAuthority()) return;
	ApplyEffectToSelf(DefaultAttributes, 1.0f);
	ApplyEffectToSelf(DefaultSeceondaryAttributes, 1.0f);

	//In DefaultSeceondaryAttributes set vital max value so after call DefaultVitalAttributes apply
	ApplyEffectToSelf(DefaultVitalAttributes, 1.0f);

	AbilitySystemComponent->AbilityActorInfoSet();
}

void AProjectGOCharacter::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || Cast<
		UGOAbilitySystemComponent>(AbilitySystemComponent)->StartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(
			GameplayEffect, ICombatInterface::GetLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
				*NewHandle.Data.Get(), AbilitySystemComponent.Get());
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
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."),
		       *FString(__FUNCTION__), *GetName());
		return;
	}
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	const FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(
		GameplayEffectClass, SelfLevel, EffectContext);
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

void AProjectGOCharacter::Die()
{
	/*
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAbilities();

		FGameplayTagContainer EffectTagToRemove;
		EffectTagToRemove.AddTag(EffectRemoveOnDeadTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagToRemove);
		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		FinishDying();
	}	 
	*/
	MulticastHandleDeath();
}

void AProjectGOCharacter::MulticastHandleDeath_Implementation()
{
	//if have weapon
	//weapon->setsimulatephysics / weapon->setenablegravity / weapon->setcollisionenabled(physicsonly)

	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->Velocity = FVector(0);
	OnCharacterDied.Broadcast(this);

	if (!HasAuthority())
	{
		HealthBar_CharacterHead->SetVisibility(false);
	}
}

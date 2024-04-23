// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectGOCharacter.h"

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
#include "Net/UnrealNetwork.h"
#include "ProjectGO/ProjectGO.h"
#include "ProjectGO/HUD/InGameHud.h"
#include "ProjectGO/Player/GOPlayerState.h"
#include "ProjectGO/PlayerController/ProjectGOPlayerController.h"
#include "ProjectGO/UI/GOUserWidgetBase.h"

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

	HealthBar_Head = CreateDefaultSubobject<UWidgetComponent>("HealthBar_Head");
	HealthBar_Head->SetupAttachment(GetRootComponent());
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
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->AbilitiesGiven)
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

void AProjectGOCharacter::InitializeAbilityValue(AGOPlayerState* PS)
{
	if (!AbilitySystemComponent.IsValid() || !AttributeSetBase.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent, AttributeSetBase Is Not Valid"));
		return;
	}
	
	AbilitySystemComponent->AbilityActorInfoSet();

	if (!HasAuthority())
	{
		if (AProjectGOPlayerController* GOPlayerController = Cast<AProjectGOPlayerController>(GetController()))
		{	
			if (AInGameHud* InGameHud = Cast<AInGameHud>(GOPlayerController->GetHUD()))
			{
				InGameHud->InitOverlay(GOPlayerController, GetPlayerState(), AbilitySystemComponent.Get(), AttributeSetBase.Get());
			}
		}	

		if(!GetPlayerState())
		{
			//Moster
			HealthBar_Head->SetWidgetClass(EnemyHealthBar_HeadClass);
		}
		else
		{
			//After check team number, separate character health bar. 
			if (const APlayerController* LocalController = UGameplayStatics::GetPlayerController(this, 0))
			{
				if(LocalController->PlayerState == PS)
				{
					//My
					HealthBar_Head->SetWidgetClass(MyHealthBar_HeadClass);
				}
				else
				{
					//Enemy
					HealthBar_Head->SetWidgetClass(EnemyHealthBar_HeadClass);
				}
			}
		}

		if (UGOUserWidgetBase* HeadWidget = Cast<UGOUserWidgetBase>(HealthBar_Head->GetUserWidgetObject()))
		{
			HeadWidget->SetWidgetController(this);
			UE_LOG(LogTemp, Error, TEXT("HeadWidget, SetWidgetController Called"));
		}
		
		AbilitySystemComponent.Get()->GetGameplayAttributeValueChangeDelegate(AttributeSetBase.Get()->GetMaxHealthAttribute()).AddLambda([&](const FOnAttributeChangeData& Data)
			{
				MaxHealthChangedSignature.Broadcast(Data.NewValue);
			});
		AbilitySystemComponent.Get()->GetGameplayAttributeValueChangeDelegate(AttributeSetBase.Get()->GetHealthAttribute()).AddLambda([&](const FOnAttributeChangeData& Data)
			{
				HealthChangedSignature.Broadcast(Data.NewValue);
			});
		MaxHealthChangedSignature.Broadcast(AttributeSetBase.Get()->GetMaxHealth());
		HealthChangedSignature.Broadcast(AttributeSetBase.Get()->GetHealth());

	}
}

FVector AProjectGOCharacter::GetCombatSocketLocation()
{
	if (!GetMesh()) return FVector();
	return GetMesh()->GetSocketLocation(WeaponTipSocketName);
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

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
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

	if(!HasAuthority())
	{
		HealthBar_Head->SetVisibility(false);
	}
}
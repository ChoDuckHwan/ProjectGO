// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Character/ProjectGOMonster.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GOAbilityBFL.h"
#include "Net/UnrealNetwork.h"
#include "Abilities/GOAbilitySystemComponent.h"
#include "Abilities/AttributeSet/GOAttributeSetBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectGO/GOGameplayTags.h"
#include "ProjectGO/AI/GOAIController.h"
#include "ProjectGO/UI/Widget/GOWidgetComponent.h"

AProjectGOMonster::AProjectGOMonster(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MonsterAbilitySystemComponent = CreateDefaultSubobject<UGOAbilitySystemComponent>(TEXT("MonsterAbilitySystemComponent"));
	MonsterAbilitySystemComponent->SetIsReplicated(true);
	MonsterAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	MonsterAttributeSetBase = CreateDefaultSubobject<UGOAttributeSetBase>(TEXT("MonsterAttributeSetBase"));
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	AbilitySystemComponent = MonsterAbilitySystemComponent;
	AttributeSetBase = MonsterAttributeSetBase;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void AProjectGOMonster::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	if(GetNetMode() != NM_Client)
	{
		AddStartupEffects();
		AddCharacterAbilities();
	}
	InitializeAbilityValue(nullptr);
	MonsterAbilitySystemComponent->RegisterGameplayTagEvent(FGOGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AProjectGOMonster::HitReactTagChanged);
		
	UGOAbilityBFL::GiveStartupAbilities(this, AbilitySystemComponent.Get());
}

void AProjectGOMonster::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AProjectGOMonster, Level);
}

void AProjectGOMonster::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if(!HasAuthority()) return;
	
	GOAIController = Cast<AGOAIController>(NewController);
	check(GOAIController.Get());
	GOAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->GetBlackboardAsset());
	GOAIController->RunBehaviorTree(BehaviorTree);		

	GOAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	GOAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), MonsterClass != ECharacterClass::Warrior);
}

void AProjectGOMonster::InitializeAttributes() const
{
	UGOAbilityBFL::InitializeDefaultAttributes(this, MonsterClass, Level, GetAbilitySystemComponent());
}

int32 AProjectGOMonster::GetLevel() const
{
	return Level;
}

void AProjectGOMonster::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	UE_LOG(LogTemp, Log, TEXT("HitReactTagChanged Called"));
	if(GOAIController.Get())
	{
		GOAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);		
	}

}

void AProjectGOMonster::Die()
{
	SetLifeSpan(RagdollLifeTime);
	Super::Die();
}

void AProjectGOMonster::InitializeAbilityValue(AGOPlayerState* PS)
{
	AbilitySystemComponent = MonsterAbilitySystemComponent;
	AttributeSetBase = MonsterAttributeSetBase;
	if (!AbilitySystemComponent.IsValid() || !AttributeSetBase.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent, AttributeSetBase Is Not Valid"));
		return;
	}
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	Super::InitializeAbilityValue(PS);
	InitializeAttributes();

	HealthBar_CharacterHead->SetWidgetClass(EnemyHealthBar_HeadClass);
}

void AProjectGOMonster::OnRep_Level(const int32& oldLevel)
{

}

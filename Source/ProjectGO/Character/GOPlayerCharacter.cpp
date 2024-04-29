// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Character/GOPlayerCharacter.h"
#include "Abilities/GOAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Abilities/AttributeSet/GOAttributeSetBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ProjectGO/GOGameplayTags.h"
#include "ProjectGO/AI/GOAIController.h"
#include "ProjectGO/Player/GOPlayerState.h"

AGOPlayerCharacter::AGOPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	AttackSphere->SetupAttachment(RootComponent);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);


	AIControllerClass = AGOAIController::StaticClass();
}

void AGOPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AGOPlayerState* PS = GetPlayerState<AGOPlayerState>();
	InitializeAbilityValue(PS);

	AddStartupEffects();
	AddCharacterAbilities();	
}

void AGOPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AGOPlayerState* PS = GetPlayerState<AGOPlayerState>();
	InitializeAbilityValue(PS);
}

int32 AGOPlayerCharacter::GetLevel() const
{
	check(GetPlayerState<AGOPlayerState>());
	return GetPlayerState<AGOPlayerState>()->GetPlayerLevel();	
}

void AGOPlayerCharacter::InitializeAbilityValue(AGOPlayerState* PS)
{
	if (PS)
	{
		AbilitySystemComponent = Cast<UGOAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		AttributeSetBase = PS->GetAttributeSetBase();
		//AI maybe cannot have controller so abilityactorinfoinit here
		if(!AbilitySystemComponent.IsValid() || !AttributeSetBase.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent, AttributeSetBase Is Not Valid"));
			return;
		}
		GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
		Super::InitializeAbilityValue(PS);
		Super::InitializeAttributes();
	}
}

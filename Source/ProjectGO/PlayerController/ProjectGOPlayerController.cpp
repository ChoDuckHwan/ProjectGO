// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectGOPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectGO/Input/GOInputComponent.h"
#include "ProjectGO/Input/DA_GOInputConfig.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "ProjectGO/Character/Abilities/GOAbilitySystemComponent.h"
#include "ProjectGO/Player/GOPlayerState.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AProjectGOPlayerController::AProjectGOPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AProjectGOPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AProjectGOPlayerController::SetConsumeMove(const bool& NewComsumeMove)
{
	bConsumeMove = NewComsumeMove;
}

const bool& AProjectGOPlayerController::GetConsumeMove() const
{
	return bConsumeMove;
}

void AProjectGOPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	TObjectPtr<UGOInputComponent> GOInputComponent = Cast<UGOInputComponent>(InputComponent);

	if(GOInputComponent)
	{
		GOInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);

		/*Mouse Click Effect*/
		GOInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AProjectGOPlayerController::OnSetDestinationTriggered);
		GOInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AProjectGOPlayerController::OnSetDestinationReleased);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
	/*
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AProjectGOPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AProjectGOPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AProjectGOPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AProjectGOPlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AProjectGOPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AProjectGOPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AProjectGOPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AProjectGOPlayerController::OnTouchReleased);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

	*/
}

void AProjectGOPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AProjectGOPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	//APawn* ControlledPawn = GetPawn();
	//if (ControlledPawn != nullptr)
	{
		//FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		//ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AProjectGOPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold && !bConsumeMove)
	{
		// We move there and spawn some particles
		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		//Server_Move(CachedDestination.X, CachedDestination.Y);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AProjectGOPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AProjectGOPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void AProjectGOPlayerController::Server_Move_Implementation(const int& LocationX, const int& LocationY)
{
	FVector MoveLocation;
	MoveLocation.X = LocationX;
	MoveLocation.Y = LocationY;
	
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MoveLocation);
}

void AProjectGOPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void AProjectGOPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!GetGOASC().IsValid()) return;
	GetGOASC()->AbilityInputTagReleased(InputTag);
}

void AProjectGOPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!GetGOASC().IsValid()) return;
	GetGOASC()->AbilityInputTagHeld(InputTag);
}

TWeakObjectPtr<UGOAbilitySystemComponent> AProjectGOPlayerController::GetGOASC()
{
	if(!GOAbilitySystemComponent.IsValid())
	{
		return Cast<UGOAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return GOAbilitySystemComponent;
}

void AProjectGOPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AGOPlayerState* PS = GetPlayerState<AGOPlayerState>();

	if (PS)
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}

//TODO Add HUD stuff
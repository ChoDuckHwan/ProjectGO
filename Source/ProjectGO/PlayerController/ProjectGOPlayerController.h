// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "ProjectGOPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
class UDA_GOInputConfig;
class UGOAbilitySystemComponent;
class UDamageTextComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class AProjectGOPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AProjectGOPlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationTouchAction;
	
	UFUNCTION(BlueprintCallable)
	void SetConsumeMove(const bool& NewComsumeMove);

	UFUNCTION(BlueprintCallable)
	const bool& GetConsumeMove() const;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);
	void ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UDA_GOInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputAction> ShiftAction;

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void OnPossess(APawn* aPawn) override;

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

	UFUNCTION(Server, Reliable)
	void Server_Move(const int& LocationX, const int& LocationY);
	void Server_Move_Implementation(const int& LocationX, const int& LocationY);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bConsumeMove = false;
private:
	FVector CachedDestination;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed	

	UFUNCTION()	void AbilityInputTagPressed(FGameplayTag InputTag);
	UFUNCTION()	void AbilityInputTagReleased(FGameplayTag InputTag);
	UFUNCTION()	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY()
	TWeakObjectPtr<UGOAbilitySystemComponent> GOAbilitySystemComponent;

	UFUNCTION()
	TWeakObjectPtr<UGOAbilitySystemComponent> GetGOASC();


	void ShiftPressed() { bShiftKeyDown = true; }
	void ShiftReleased() { bShiftKeyDown = false; }

	bool bShiftKeyDown = false;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};



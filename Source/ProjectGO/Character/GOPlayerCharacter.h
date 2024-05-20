// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectGO/Character/ProjectGOCharacter.h"
#include "ProjectGO/Interaction/PlayerInterface.h"
#include "GOPlayerCharacter.generated.h"

/**
 * 
 */
class AGOPlayerState;

UCLASS()
class PROJECTGO_API AGOPlayerCharacter : public AProjectGOCharacter, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AGOPlayerCharacter(const class FObjectInitializer& ObjectInitializer);


	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;
	
	/* Combat Interface*/
	virtual int32 GetLevel_Implementation() const;
	/* Combat Interface*/

	/* Player Interface */
	virtual void AddToXP_Implementation(const int32& XP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 FindLevelForXP_Implementation(const int32& InXP) const override;

	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoint) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoint) override;
	/* Player Interface */

protected:
	virtual void InitializeAbilityValue(AGOPlayerState* PS) override;
private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> AttackSphere;
};

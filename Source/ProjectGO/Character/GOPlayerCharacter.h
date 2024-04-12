// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectGO/Character/ProjectGOCharacter.h"
#include "GOPlayerCharacter.generated.h"

/**
 * 
 */
class AGOPlayerState;

UCLASS()
class PROJECTGO_API AGOPlayerCharacter : public AProjectGOCharacter
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
	virtual int32 GetLevel() override;
	/* Combat Interface*/
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

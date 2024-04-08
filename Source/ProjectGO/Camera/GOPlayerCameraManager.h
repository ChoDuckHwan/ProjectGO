// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "GOPlayerCameraManager.generated.h"

/**
 *
 */
UCLASS()
class PROJECTGO_API AGOPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	AGOPlayerCameraManager(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY()
	FName CameraMoveType = FName(TEXT("Mouse"));

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float CameraMoveScale = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float FOVMin = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float FOVMax = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float FOVScale = 50.0f;

	void InitializeTopViewCamera(class AActor* NewViewTarget);

public:
	virtual void SetViewTarget(class AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams()) override;

	UFUNCTION(BlueprintCallable)
	void SetCameraDistance(const float& NewDistance);
	
	virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetTopViewCameraLocation(const FVector& NewLocation);

	UFUNCTION(BlueprintCallable)
	void SetTopViewCameraRotation(const FRotator& OutRotation);

	UFUNCTION(BlueprintCallable)
	const FVector& GetTopViewCameraLocation();

	UFUNCTION(BlueprintCallable)
	const FRotator& GetTopViewCameraRotation();

	UFUNCTION(BlueprintCallable)
	const float& GetCameraMoveScale();

	UFUNCTION(BlueprintCallable)
	void SetCameraMoveScale(const float& NewCameraScale);

	UFUNCTION(BlueprintCallable)
	void AddCameraLocation(const FVector& AddLocation);

	UFUNCTION(BlueprintCallable)
	virtual void SetFOV(float NewFOV) override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetCameraFOVSmooth(const float& WheelValue);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void CameraMove_MouseX();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void CameraMove_MouseY();
};

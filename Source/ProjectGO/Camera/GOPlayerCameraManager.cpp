// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Camera/GOPlayerCameraManager.h"

AGOPlayerCameraManager::AGOPlayerCameraManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static const FName NAME_Fixed = FName(TEXT("Fixed"));
	static const FName NAME_ThirdPerson = FName(TEXT("ThirdPerson"));
	static const FName NAME_FreeCam = FName(TEXT("FreeCam"));
	static const FName NAME_FreeCam_Default = FName(TEXT("FreeCam_Default"));
	static const FName NAME_FirstPerson = FName(TEXT("FirstPerson"));

	//CameraStyle = NAME_FreeCam;

}

void AGOPlayerCameraManager::SetViewTarget(AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams)
{
	Super::SetViewTarget(NewViewTarget, TransitionParams);

	InitializeTopViewCamera(NewViewTarget);
}

void AGOPlayerCameraManager::SetCameraDistance(const float& NewDistance)
{
	FreeCamDistance = NewDistance;
}

void AGOPlayerCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime)
{
	if (OutVT.Target)
	{
		static const FName Mouse = FName(TEXT("Mouse"));
		static const FName TargetFixed = FName(TEXT("TargetFixed"));

		if(CameraMoveType == Mouse)		
		{
			OutVT.POV.Location = Location;
			OutVT.POV.Rotation = Rotation;
			OutVT.POV.FOV = GetFOVAngle();
		}
		else
		{
			OutVT.Target->CalcCamera(DeltaTime, OutVT.POV);
		}
	}
}

void AGOPlayerCameraManager::SetTopViewCameraLocation(const FVector& NewLocation)
{
	Location = NewLocation;
}

void AGOPlayerCameraManager::SetTopViewCameraRotation(const FRotator& NewRotation)
{
	Rotation = NewRotation;
}

const FVector& AGOPlayerCameraManager::GetTopViewCameraLocation()
{
	return Location;
}

const FRotator& AGOPlayerCameraManager::GetTopViewCameraRotation()
{
	return Rotation;
}

const float& AGOPlayerCameraManager::GetCameraMoveScale()
{
	return CameraMoveScale;
}

void AGOPlayerCameraManager::SetCameraMoveScale(const float& NewCameraScale)
{
	CameraMoveScale = NewCameraScale;
}

void AGOPlayerCameraManager::AddCameraLocation(const FVector& AddLocation)
{
	Location += AddLocation;
}

void AGOPlayerCameraManager::SetFOV(float NewFOV)
{
	Super::SetFOV(NewFOV);
}


void AGOPlayerCameraManager::InitializeTopViewCamera(AActor* NewViewTarget)
{
	struct FMinimalViewInfo NewTargetOutPov;
	NewViewTarget->CalcCamera(0.f, NewTargetOutPov);

	Location = NewTargetOutPov.Location;
	Rotation = NewTargetOutPov.Rotation;
	SetActorLocationAndRotation(Location, Rotation);
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectGOGameMode.h"
#include "ProjectGO/Character/ProjectGOCharacter.h"
#include "ProjectGO/PlayerController/ProjectGOPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AProjectGOGameMode::AProjectGOGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AProjectGOPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/BP_CharacterBase"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Controller/BP_ControllerBase"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}
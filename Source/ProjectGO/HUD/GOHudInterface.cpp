// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/HUD/GOHudInterface.h"

// Add default functionality here for any IGOHudInterface functions that are not pure virtual.
void IGOHudInterface::BP_OnChangeMouseCursorType_Implementation(TSubclassOf<UGOMouseCursorWidget> MouseCursorType)
{
	BP_OnChangeMouseCursorType(MouseCursorType);
}

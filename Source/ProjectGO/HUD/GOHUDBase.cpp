// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/HUD/GOHUDBase.h"

void AGOHUDBase::BP_OnChangeMouseCursorType_Implementation(TSubclassOf<UGOMouseCursorWidget> MouseCursorType)
{
	Execute_BP_OnChangeMouseCursorType(this, MouseCursorType);
}

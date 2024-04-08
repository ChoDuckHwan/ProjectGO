// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/UI/Character/UW_CharacterInfoBar.h"

void UUW_CharacterInfoBar::SetOwnerCharacter(AActor* NewOwnerActor)
{
	OwnerActor = NewOwnerActor;
}

const AActor* UUW_CharacterInfoBar::GetOwnerActor() const
{
	return OwnerActor;
}

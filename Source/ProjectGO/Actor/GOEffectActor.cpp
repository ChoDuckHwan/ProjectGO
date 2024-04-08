// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Actor/GOEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "ProjectGO/Character/Abilities/GOAbilitySystemComponent.h"

// Sets default values
AGOEffectActor::AGOEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

// Called when the game starts or when spawned
void AGOEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGOEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UGOAbilitySystemComponent* TargetASC = Cast<UGOAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target));
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

// Called every frame
void AGOEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


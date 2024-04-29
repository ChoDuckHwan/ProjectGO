// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Actor/GOEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"
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

void AGOEffectActor::OnOverlap(AActor* TargetActor)
{
	if(TargetActor->ActorHasTag(FName("Monster")) && !bApplyEffectsToEnemies) return;

	if(InstantEffectApplicationPoilicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if(DurationEffectApplicationPoilicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPoilicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AGOEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if(TargetActor->ActorHasTag(FName("Monster")) && !bApplyEffectsToEnemies) return;

	if(InstantEffectApplicationPoilicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPoilicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPoilicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	if(InfiniteEffectRemovalPoilicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;
		
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for(const auto& ActiveEffectHandle : ActiveEffectHandles)
		{
			if(ActiveEffectHandle.Value == TargetASC)
			{
				TargetASC->RemoveActiveGameplayEffect(ActiveEffectHandle.Key, 1);
				HandlesToRemove.Add(ActiveEffectHandle.Key);
			}
		}
		for(auto& RemoveHandle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(RemoveHandle);
		}
	}
}

void AGOEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{	
	if(TargetActor->ActorHasTag(FName("Monster")) && !bApplyEffectsToEnemies) return;
	UGOAbilitySystemComponent* TargetASC = Cast<UGOAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfiniteDuration = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if(bIsInfiniteDuration && InfiniteEffectRemovalPoilicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
	
	if(!bIsInfiniteDuration)
	{
		Destroy();
	}
}

// Called every frame
void AGOEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


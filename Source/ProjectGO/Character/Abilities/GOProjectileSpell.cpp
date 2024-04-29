// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Character/Abilities/GOProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Projects.h"
#include "GameFramework/PlayerState.h"
#include "ProjectGO/GOGameplayTags.h"
#include "ProjectGO/Actor/GOProjectile.h"
#include "ProjectGO/Interaction/CombatInterface.h"

void UGOProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	

}

void UGOProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{	
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		Rotation.Pitch = 0.f;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);		
		SpawnTransform.SetRotation(Rotation.Quaternion());

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *Cast<APawn>(GetOwningActorFromActorInfo())->GetName());

		AGOProjectile* Projectile = GetWorld()->SpawnActorDeferred<AGOProjectile>(ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//TODO: Give the Projectle a Gameplay Effect Spec for causing Damage.
			
		if (Projectile)
		{
			const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
			FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
			
			EffectContextHandle.SetAbility(this);
			EffectContextHandle.AddSourceObject(Projectile);
			TArray<TWeakObjectPtr<AActor>> Actors;
			Actors.Add(Projectile);
			EffectContextHandle.AddActors(Actors);
			FHitResult Hitresult;
			Hitresult.Location = ProjectileTargetLocation;
			EffectContextHandle.AddHitResult(Hitresult);
			
			
			const FGameplayEffectSpecHandle GameplayEffectSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
			const FGOGameplayTags GamePlayTags = FGOGameplayTags::Get();
			for(const auto& DamageType : DamageTypes)
			{
				const float ScaleDamage = DamageType.Value.GetValueAtLevel(GetAbilityLevel());
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Arrow Damage %f"), ScaleDamage));
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GameplayEffectSpecHandle, DamageType.Key, ScaleDamage);
			}			
			//CT_Damage Damage by Level
			Projectile->DamageEffectSpecHandle = GameplayEffectSpecHandle;
			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}
	

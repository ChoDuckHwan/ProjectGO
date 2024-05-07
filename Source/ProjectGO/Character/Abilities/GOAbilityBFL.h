// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProjectGO/Character/Abilities/Data/DA_AttributeInfo.h"
#include "ProjectGO/Character/Abilities/Data/CharacterClassInfo.h"
#include "GOAbilityBFL.generated.h"

class UGOAttributeWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class PROJECTGO_API UGOAbilityBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|WidgetController")
	static UGOAttributeWidgetController* GetAttributeWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|CharacterClassDefaults")
	static void GiveCommonAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);	

	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|CharacterClassInfo")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|GameplayeEffect")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|GameplayeEffect")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContext);

	/*!!!UPARAM(ref)!!!*/
	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|GameplayeEffect")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, bool InIsBlockedHit);
	
	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|GameplayeEffect")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContext, bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithInRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereCenter);

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(AActor* FristActor, AActor* SecondActor);
};

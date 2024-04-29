#pragma once
#include "GameplayEffectTypes.h"
#include "GOAbilityTypes.generated.h"
USTRUCT(BlueprintType)
struct FGOGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsCriticalHit() const { return bCriticalHit; }
	bool IsBlockedHit() const { return bBlockedHit; }
	void SetIsCriticalHit(bool InCriticalHit) { bCriticalHit = InCriticalHit; }
	void SetIsBlockedHit(bool InBlockedHit) { bBlockedHit = InBlockedHit; }
	
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGOGameplayEffectContext::StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FGOGameplayEffectContext* Duplicate() const
	{
		FGOGameplayEffectContext* NewContext = new FGOGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bCriticalHit = false;

	UPROPERTY(BlueprintReadOnly)
	bool bBlockedHit = false;
};

template<>
struct TStructOpsTypeTraits< FGOGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FGOGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};

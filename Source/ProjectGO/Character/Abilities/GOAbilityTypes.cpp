#include "GOAbilityTypes.h"

bool FGOGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;
	int BitPos = 0;
	int CheckBitPos = BitPos;
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << BitPos++;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid() )
		{
			RepBits |= 1 << BitPos++;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << BitPos++;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << BitPos++;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << BitPos++;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << BitPos++;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << BitPos++;
		}
		if(IsBlockedHit())
		{
			RepBits |= 1 << BitPos++;
		}
		if(IsCriticalHit())
		{
			RepBits |= 1 << BitPos++;
		}
	}

	Ar.SerializeBits(&RepBits, BitPos);

	if (RepBits & (1 << CheckBitPos++))	//0
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << CheckBitPos++))	//1
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << CheckBitPos++))	//2
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << CheckBitPos++))	//3
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << CheckBitPos++))	//4
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << CheckBitPos++))	//5
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << CheckBitPos++))	//6
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	if (RepBits & (1 << CheckBitPos++))	//7
	{
		Ar << bBlockedHit;		
	}
	if (RepBits & (1 << CheckBitPos++))	//8
	{
		Ar << bCriticalHit;		
	}
	else
	{
		bHasWorldOrigin = false;
	}

	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}	
	
	bOutSuccess = true;
	return true;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Character/Abilities/Data/LevelupInfo.h"

int32 ULevelupInfo::FindLevelForXP(int32 XP) const
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		//LevelupInfos[1] = Level 1 Information
		// LevelupInfos[2] = Level 2 Information
		if (LevelupInfos.Num() - 1 <= Level) return Level;

		if (XP >= LevelupInfos[Level].LevelupRequirement)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}

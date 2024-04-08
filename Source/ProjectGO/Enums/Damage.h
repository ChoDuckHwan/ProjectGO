#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGOAbilityID : uint8
{

	None			UMETA(DisplayName = "None"),
	Confirm			UMETA(DisplayName = "Confirm"),
	Cancel			UMETA(DisplayName = "Cancel"),
	AbilityPassive	UMETA(DisplayName = "Passive"),
	AbilityQ		UMETA(DisplayName = "Q"),
	AbilityW		UMETA(DisplayName = "W"),
	AbilityE		UMETA(DisplayName = "E"),
	AbilityR		UMETA(DisplayName = "R"),
};
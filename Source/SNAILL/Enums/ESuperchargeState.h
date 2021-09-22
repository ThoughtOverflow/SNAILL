// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(Blueprintable, BlueprintType)
enum class ESuperchargeState : uint8
{
	ESS_Charged = 0 UMETA(DisplayName = "Charged"),
	ESS_Discharged = 1 UMETA(DisplayName = "Discharged"),
	ESS_Disabled = 2 UMETA(DisplayName = "Disabled")
};

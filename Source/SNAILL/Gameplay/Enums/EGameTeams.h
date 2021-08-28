// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EGameTeams.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class EGameTeams : uint8 {
	EGT_TeamNone = 0 UMETA(DisplayName = "None"),
	EGT_TeamA = 1 UMETA(DisplayName = "Team A"),
	EGT_TeamB = 2 UMETA(DisplayName = "Team B")
};
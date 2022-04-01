// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EThrowableAnimType.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class EThrowableAnimType : uint8 {
	STOP_ANIM = 0 UMETA(DisplayName = "Stop Animation"),
	PLAY_INIT = 1 UMETA(DisplayName = "Play Initial Animation"),
	PLAY_THROW = 2 UMETA(DisplayName = "Play Throw Animation")
};
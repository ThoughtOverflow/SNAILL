// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SNAILLGameState.h"
#include "GameFramework/PlayerState.h"
#include "SNAILLPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SNAILL_API ASNAILLPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	ASNAILLPlayerState();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated)
		FString PlayerName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated)
		int32 PlayerCurrentKills;

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	
};

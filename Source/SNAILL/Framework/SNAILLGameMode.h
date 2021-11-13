// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SNAILLGameState.h"
#include "GameFramework/GameModeBase.h"
#include "SNAILL/Character/PlayerCharacter.h"
#include "SNAILL/Gameplay/Enums/EGameTeams.h"
#include "GameFramework/PlayerStart.h"

#include "SNAILLGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SNAILL_API ASNAILLGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ASNAILLGameMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ASNAILLGameState* SnailGameState;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<APlayerCharacter> TeamACharacter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<APlayerCharacter> TeamBCharacter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<APlayerStart> TeamAStartLocation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<APlayerStart> TeamBStartLocation;

public:

	UFUNCTION(BlueprintCallable)
		void JoinTeam(EGameTeams Team, ASNAILLPlayerController* Player);
	UFUNCTION(BlueprintCallable)
		void ProjectileHit(AActor* Shooter, AActor* Target, int32 DamageToDeal, bool bCanProjectileDamageAllies);
	UFUNCTION(BlueprintCallable)
		void Respawn(ASNAILLPlayerController* PlayerToRespawn);
	UFUNCTION(BlueprintCallable)
		void EndGame(EGameTeams WinningTeam);
	UFUNCTION(BlueprintCallable)
		void SetKillerDataForEnemy(APlayerCharacter* Player, FString DamageDealerName);
	
	
};

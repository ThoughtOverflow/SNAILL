// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SNAILLGameState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTeamSelectionWidgetRefreshRequestCreated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerHUDWidgetRefreshRequestCreated);

UCLASS()
class SNAILL_API ASNAILLGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASNAILLGameState();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing=OnRep_TeamAPlayers)
		TArray<class ASNAILLPlayerController*> TeamAPlayers;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing=OnRep_TeamBPlayers)
		TArray<class ASNAILLPlayerController*> TeamBPlayers;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing=OnRep_TeamAKillScore)
		int32 TeamAKillScore;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing=OnRep_TeamBKillScore)
	int32 TeamBKillScore;

	//Game Default Rules:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated)
	int32 MaxKillCountToWin;

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
		void OnRep_TeamAPlayers();
	UFUNCTION()
		void OnRep_TeamBPlayers();
	UFUNCTION()
		void OnRep_TeamAKillScore();
	UFUNCTION()
		void OnRep_TeamBKillScore();

	UPROPERTY(BlueprintAssignable)
		FOnTeamSelectionWidgetRefreshRequestCreated OnTeamSelectionWidgetRefreshRequestCreated;
	UPROPERTY(BlueprintAssignable)
		FOnPlayerHUDWidgetRefreshRequestCreated OnPlayerHUDWidgetRefreshRequestCreated;

public:

	UFUNCTION(Client, Unreliable)
			void Client_RefreshTeamSelectionWidget();
	
};

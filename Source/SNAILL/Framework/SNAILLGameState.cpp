// Fill out your copyright notice in the Description page of Project Settings.


#include "SNAILLGameState.h"

#include "SNAILLGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ASNAILLGameState::ASNAILLGameState()
{
	TeamAKillScore = 0;
	TeamBKillScore = 0;
	MaxKillCountToWin = 50;
}

void ASNAILLGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASNAILLGameState, TeamAPlayers);
	DOREPLIFETIME(ASNAILLGameState, TeamBPlayers);
	DOREPLIFETIME(ASNAILLGameState, TeamAKillScore);
	DOREPLIFETIME(ASNAILLGameState, TeamBKillScore);
	DOREPLIFETIME(ASNAILLGameState, MaxKillCountToWin)
}

void ASNAILLGameState::OnRep_TeamAPlayers()
{
	OnTeamSelectionWidgetRefreshRequestCreated.Broadcast();
}

void ASNAILLGameState::OnRep_TeamBPlayers()
{
	OnTeamSelectionWidgetRefreshRequestCreated.Broadcast();
}

void ASNAILLGameState::OnRep_TeamAKillScore()
{
	OnPlayerHUDWidgetRefreshRequestCreated.Broadcast();
}

void ASNAILLGameState::OnRep_TeamBKillScore()
{
	OnPlayerHUDWidgetRefreshRequestCreated.Broadcast();
}

void ASNAILLGameState::Client_RefreshTeamSelectionWidget_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ITRAN"));
	OnTeamSelectionWidgetRefreshRequestCreated.Broadcast();
}

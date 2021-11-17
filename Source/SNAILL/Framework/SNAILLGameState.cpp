// Fill out your copyright notice in the Description page of Project Settings.


#include "SNAILLGameState.h"

#include "SNAILLGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ASNAILLGameState::ASNAILLGameState()
{
	TeamAKillScore = 0;
	TeamBKillScore = 0;
	MaxKillCountToWin = 30;
	SnailCollectorKillRequirement = 5;
}

void ASNAILLGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASNAILLGameState, TeamAPlayers);
	DOREPLIFETIME(ASNAILLGameState, TeamBPlayers);
	DOREPLIFETIME(ASNAILLGameState, TeamAKillScore);
	DOREPLIFETIME(ASNAILLGameState, TeamBKillScore);
	DOREPLIFETIME(ASNAILLGameState, MaxKillCountToWin)
	DOREPLIFETIME(ASNAILLGameState, SnailCollectorKillRequirement)
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

void ASNAILLGameState::RefreshPlayerScoreIndicatorForServer()
{
	OnRep_TeamAKillScore();
	OnRep_TeamBKillScore();
}

EGameTeams ASNAILLGameState::GetPlayerTeam(ASNAILLPlayerController* Player)
{
		
		if(TeamAPlayers.Contains(Player)) {
		
			return EGameTeams::EGT_TeamA;
		
		}else if(TeamBPlayers.Contains(Player)) {
		
			return EGameTeams::EGT_TeamB;
		
		}else {
			return EGameTeams::EGT_TeamNone;
		}
	
}

void ASNAILLGameState::ChangeTeamScore(EGameTeams Team, int32 deltaScore)
{
	if(HasAuthority())
	{
		switch (Team)
		{
			case EGameTeams::EGT_TeamA:
				TeamAKillScore += deltaScore;
			break;
			case EGameTeams::EGT_TeamB:
				TeamBKillScore += deltaScore;
			break;
			case EGameTeams::EGT_TeamNone:
			break;
		}
		if(GetNetMode() != NM_DedicatedServer)
		{
			OnRep_TeamAKillScore();
			OnRep_TeamBKillScore();
		}
	}
}


void ASNAILLGameState::Client_RefreshTeamSelectionWidget_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ITRAN"));
	OnTeamSelectionWidgetRefreshRequestCreated.Broadcast();
}

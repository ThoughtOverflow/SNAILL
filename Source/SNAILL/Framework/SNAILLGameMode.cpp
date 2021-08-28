// Fill out your copyright notice in the Description page of Project Settings.


#include "SNAILLGameMode.h"

#include "Kismet/GameplayStatics.h"

ASNAILLGameMode::ASNAILLGameMode()
{
}

void ASNAILLGameMode::JoinTeam(EGameTeams Team, ASNAILLPlayerController* Player)
{
	if(!SnailGameState)
	{
		SnailGameState = GetGameState<ASNAILLGameState>();
	}
	
	if(Team==EGameTeams::EGT_TeamA)
	{
		SnailGameState->TeamAPlayers.Add(Player);
		SnailGameState->Client_RefreshTeamSelectionWidget();
		//Character Spawn Logic:
		if(TeamACharacter)
		{
			TArray<AActor*> TeamAStart;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), TeamAStartLocation, TeamAStart);
			int randSpawnLoc = FMath::RandRange(0, TeamAStart.Num()-1);
			if(APlayerStart* SelectedStart = Cast<APlayerStart>(TeamAStart[randSpawnLoc]))
			{
				Player->ClientSetRotation(SelectedStart->GetActorRotation());
				APlayerCharacter* SpawnedPlayer = GetWorld()->SpawnActor<APlayerCharacter>(TeamACharacter, SelectedStart->GetActorLocation(), SelectedStart->GetActorRotation());
				Player->Possess(SpawnedPlayer);
				SpawnedPlayer->DisplayBasicUI();
			}
		}
		
	}else if(Team==EGameTeams::EGT_TeamB)
	{
		SnailGameState->TeamBPlayers.Add(Player);
		SnailGameState->Client_RefreshTeamSelectionWidget();
		if(TeamBCharacter)
		{
			TArray<AActor*> TeamBStart;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), TeamBStartLocation, TeamBStart);
			int randSpawnLoc = FMath::RandRange(0, TeamBStart.Num()-1);
			if(APlayerStart* SelectedStart = Cast<APlayerStart>(TeamBStart[randSpawnLoc]))
			{
				Player->ClientSetRotation(SelectedStart->GetActorRotation());
				APlayerCharacter* SpawnedPlayer = GetWorld()->SpawnActor<APlayerCharacter>(TeamBCharacter, SelectedStart->GetActorLocation(), SelectedStart->GetActorRotation());
				Player->Possess(SpawnedPlayer);
				SpawnedPlayer->DisplayBasicUI();
			}
		}
	}
}

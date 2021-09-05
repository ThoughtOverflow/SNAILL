// Fill out your copyright notice in the Description page of Project Settings.


#include "SNAILLGameMode.h"

#include "SNAILLPlayerState.h"
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
				SpawnedPlayer->bIsPlayerDead = false;
				Player->Possess(SpawnedPlayer);
				SpawnedPlayer->SelectDefWeaponTMP();
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
				SpawnedPlayer->SelectDefWeaponTMP();
				SpawnedPlayer->DisplayBasicUI();
			}
		}
	}
}

void ASNAILLGameMode::ProjectileHit(AActor* Shooter, AActor* Target, int32 DamageToDeal, bool bCanProjectileDamageAllies)
{
	if(Shooter && Target)
	{
		if(APlayerCharacter* TargetPlayer = Cast<APlayerCharacter>(Target))
		{
			APlayerCharacter* ShooterPlayer = Cast<APlayerCharacter>(Shooter);
			//SnailGameState = GetGameState<ASNAILLGameState>();
			if(bCanProjectileDamageAllies)
			{

				TargetPlayer->ChangePlayerHealth(DamageToDeal * -1);
				EGameTeams ShooterTeam = SnailGameState->GetPlayerTeam(Cast<ASNAILLPlayerController>(ShooterPlayer->GetController()));
				APlayerCharacter* KilledPlayer = Cast<APlayerCharacter>(Target);
				EGameTeams KilledTeam = SnailGameState->GetPlayerTeam(Cast<ASNAILLPlayerController>(KilledPlayer->GetController()));

				//Double Check Kill Boolean:
				if(KilledPlayer->bIsPlayerDead && ShooterTeam!=KilledTeam)
				{
					//TODO: Kill Scoring Logic
					switch (ShooterTeam)
					{
						case EGameTeams::EGT_TeamA:
						SnailGameState->TeamAKillScore++;
						SnailGameState->RefreshPlayerScoreIndicatorForServer();
						if(SnailGameState->TeamAKillScore >= SnailGameState->MaxKillCountToWin)
						{
							EndGame(EGameTeams::EGT_TeamA);
							return;
						}
						break;
						case EGameTeams::EGT_TeamB:
						SnailGameState->TeamBKillScore++;
						SnailGameState->RefreshPlayerScoreIndicatorForServer();
						if(SnailGameState->TeamBKillScore >= SnailGameState->MaxKillCountToWin)
						{
							EndGame(EGameTeams::EGT_TeamB);
							return;
						}
						break;
						case EGameTeams::EGT_TeamNone:
						break;
					}
					Cast<ASNAILLPlayerState>(ShooterPlayer->GetPlayerState())->PlayerCurrentKills++;
					KilledPlayer->GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
					
				}
				
			}else
			{
				if(SnailGameState->GetPlayerTeam(Cast<ASNAILLPlayerController>(ShooterPlayer->GetController()))!=SnailGameState->GetPlayerTeam(Cast<ASNAILLPlayerController>(TargetPlayer->GetController())))
				{
					TargetPlayer->ChangePlayerHealth(DamageToDeal * -1);
					EGameTeams Team = SnailGameState->GetPlayerTeam(Cast<ASNAILLPlayerController>(ShooterPlayer->GetController()));
					APlayerCharacter* KilledPlayer = Cast<APlayerCharacter>(Target);
					KilledPlayer->GetPlayerState<ASNAILLPlayerState>()->KillerName = ShooterPlayer->GetPlayerState<ASNAILLPlayerState>()->GetPlayerName();
					UE_LOG(LogTemp, Warning, TEXT("SET PLAYER STATE VAR"));
					//Double Check Kill Boolean:
					if(KilledPlayer->bIsPlayerDead)
					{
						//TODO: Kill Scoring Logic
						switch (Team)
						{
						case EGameTeams::EGT_TeamA:
							SnailGameState->TeamAKillScore++;
							SnailGameState->RefreshPlayerScoreIndicatorForServer();
							if(SnailGameState->TeamAKillScore >= SnailGameState->MaxKillCountToWin)
							{
								EndGame(EGameTeams::EGT_TeamA);
								return;
							}
							break;
						case EGameTeams::EGT_TeamB:
							SnailGameState->TeamBKillScore++;
							SnailGameState->RefreshPlayerScoreIndicatorForServer();
							if(SnailGameState->TeamBKillScore >= SnailGameState->MaxKillCountToWin)
							{
								EndGame(EGameTeams::EGT_TeamB);
								return;
							}
							break;
						case EGameTeams::EGT_TeamNone:
							break;
						}
						Cast<ASNAILLPlayerState>(ShooterPlayer->GetPlayerState())->PlayerCurrentKills++;
						KilledPlayer->GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
					}
				}
			}
			
		}
	}
}

void ASNAILLGameMode::Respawn(ASNAILLPlayerController* PlayerToRespawn)
{
	if(APlayerCharacter* ToDestroy = Cast<APlayerCharacter>(PlayerToRespawn->GetPawn()))
	{
		if(ToDestroy->CurrentWeapon)
		{
			ToDestroy->CurrentWeapon->Destroy();
		}
		ToDestroy->Destroy();
	}
	
	//if(PlayerToRespawn->GetPawn())
	//{
		//PlayerToRespawn->UnPossess();
	//}

	//SnailGameState = GetGameState<ASNAILLGameState>();
	TArray<AActor*> TeamStart;
	TSubclassOf<APlayerCharacter> TeamCharacter;
	switch (SnailGameState->GetPlayerTeam(PlayerToRespawn))
	{
		case EGameTeams::EGT_TeamA:
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), TeamAStartLocation, TeamStart);
			TeamCharacter = TeamACharacter;
		break;
		case EGameTeams::EGT_TeamB:
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), TeamBStartLocation, TeamStart);
			TeamCharacter = TeamBCharacter;
		break;
		case EGameTeams::EGT_TeamNone:
		break;
	}
	
	int randSpawnLoc = FMath::RandRange(0, TeamStart.Num()-1);
	if(APlayerStart* SelectedStart = Cast<APlayerStart>(TeamStart[randSpawnLoc]))
	{
		PlayerToRespawn->ClientSetRotation(SelectedStart->GetActorRotation());
		APlayerCharacter* SpawnedPlayer = GetWorld()->SpawnActor<APlayerCharacter>(TeamCharacter, SelectedStart->GetActorLocation(), SelectedStart->GetActorRotation());
		SpawnedPlayer->bIsPlayerDead = false;
		PlayerToRespawn->Possess(SpawnedPlayer);
		SpawnedPlayer->SelectDefWeaponTMP();
		SpawnedPlayer->DisplayBasicUI();
	}
}

void ASNAILLGameMode::EndGame(EGameTeams WinningTeam)
{

	UE_LOG(LogTemp, Warning, TEXT("ASDASDA"));

	if(WinningTeam==EGameTeams::EGT_TeamA)
	{
		for(ASNAILLPlayerController* Controller : SnailGameState->TeamAPlayers)
		{
			// Controller->ClientIgnoreLookInput(true);
			// Controller->ClientIgnoreMoveInput(true);
			//Controller->TogglePlayerDeathScreen(false);
			Controller->ShowGameEndScreen(true);
		}
		for(ASNAILLPlayerController* Controller : SnailGameState->TeamBPlayers)
		{
			// Controller->ClientIgnoreLookInput(true);
			// Controller->ClientIgnoreMoveInput(true);
			//Controller->TogglePlayerDeathScreen(false);
			Controller->ShowGameEndScreen(false);
		}
	}else if(WinningTeam==EGameTeams::EGT_TeamB)
	{
		for(ASNAILLPlayerController* Controller : SnailGameState->TeamAPlayers)
		{
			// Controller->ClientIgnoreLookInput(true);
			// Controller->ClientIgnoreMoveInput(true);
			//Controller->TogglePlayerDeathScreen(false);
			Controller->ShowGameEndScreen(false);
		}
		for(ASNAILLPlayerController* Controller : SnailGameState->TeamBPlayers)
		{
			// Controller->ClientIgnoreLookInput(true);
			// Controller->ClientIgnoreMoveInput(true);
			//Controller->TogglePlayerDeathScreen(false);
			Controller->ShowGameEndScreen(true);
		}
	}
	
}
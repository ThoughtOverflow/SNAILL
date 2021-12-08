// Fill out your copyright notice in the Description page of Project Settings.


#include "SNAILLGameMode.h"

#include "SNAILLPlayerState.h"
#include "Kismet/GameplayStatics.h"

ASNAILLGameMode::ASNAILLGameMode()
{
}

void ASNAILLGameMode::Logout(AController* Exiting)
{
		Super::Logout(Exiting);
	if(ASNAILLPlayerController* C = Cast<ASNAILLPlayerController>(Exiting))
	{
		UE_LOG(LogTemp, Warning, TEXT("EXIT"));
		switch (GetGameState<ASNAILLGameState>()->GetPlayerTeam(C))
		{
		case EGameTeams::EGT_TeamA:
				GetGameState<ASNAILLGameState>()->TeamAPlayers.Remove(C);
			break;
		case EGameTeams::EGT_TeamB:
			GetGameState<ASNAILLGameState>()->TeamAPlayers.Remove(C);
			break;
		case EGameTeams::EGT_TeamNone:
			break;
						
		}
		GetGameState<ASNAILLGameState>()->OnRep_TeamAPlayers();
		GetGameState<ASNAILLGameState>()->OnRep_TeamBPlayers();
		GetGameState<ASNAILLGameState>()->Client_RefreshTeamSelectionWidget();
	}
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
				SpawnedPlayer->DisplayBasicUI();
				SpawnedPlayer->SelectDefWeaponTMP();
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
				SpawnedPlayer->SelectDefWeaponTMP();
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

				EGameTeams ShooterTeam = SnailGameState->GetPlayerTeam(Cast<ASNAILLPlayerController>(ShooterPlayer->GetController()));
				APlayerCharacter* KilledPlayer = Cast<APlayerCharacter>(Target);
				EGameTeams KilledTeam = SnailGameState->GetPlayerTeam(Cast<ASNAILLPlayerController>(KilledPlayer->GetController()));
				KilledPlayer->GetPlayerState<ASNAILLPlayerState>()->KillerName = ShooterPlayer->GetPlayerState<ASNAILLPlayerState>()->PlayerName;
				KilledPlayer->GetPlayerState<ASNAILLPlayerState>()->ForceNetUpdate();
				UE_LOG(LogTemp, Warning, TEXT("SET PLAYER STATE VAR TO: %s"), *KilledPlayer->GetPlayerState<ASNAILLPlayerState>()->KillerName);

				// TargetPlayer->ChangePlayerHealth(DamageToDeal * -1);
				if(Cast<IDamageHandler>(TargetPlayer))
				{
					Cast<IDamageHandler>(TargetPlayer)->ChangeObjectHealth(DamageToDeal * -1);
				}
				
				//Double Check Kill Boolean:
				if(KilledPlayer->bIsPlayerDead)
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
					if(ShooterTeam!=KilledTeam) {
						
					Cast<ASNAILLPlayerState>(ShooterPlayer->GetPlayerState())->PlayerCurrentKills++;
					KilledPlayer->GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
					if(!ShooterPlayer->bIsSnailCollectorAvailable)
					{
						ShooterPlayer->SnailCollectorKillRegistry++;
						if(ShooterPlayer->SnailCollectorKillRegistry >= GetGameState<ASNAILLGameState>()->SnailCollectorKillRequirement)
						{
							ShooterPlayer->bIsSnailCollectorAvailable = true;
							ShooterPlayer->Client_DisplaySnailCollectorAvailability();
						}
					}
	
				}
				}
				
			}else
			{
				if(SnailGameState->GetPlayerTeam(Cast<ASNAILLPlayerController>(ShooterPlayer->GetController()))!=SnailGameState->GetPlayerTeam(Cast<ASNAILLPlayerController>(TargetPlayer->GetController())))
				{
					EGameTeams Team = SnailGameState->GetPlayerTeam(Cast<ASNAILLPlayerController>(ShooterPlayer->GetController()));
					APlayerCharacter* KilledPlayer = Cast<APlayerCharacter>(Target);
					KilledPlayer->GetPlayerState<ASNAILLPlayerState>()->KillerName = ShooterPlayer->GetPlayerState<ASNAILLPlayerState>()->PlayerName;
					KilledPlayer->GetPlayerState<ASNAILLPlayerState>()->ForceNetUpdate();
					UE_LOG(LogTemp, Warning, TEXT("SET PLAYER STATE VAR TO: %s"), *KilledPlayer->GetPlayerState<ASNAILLPlayerState>()->KillerName);

					// TargetPlayer->ChangePlayerHealth(DamageToDeal * -1);
					if(Cast<IDamageHandler>(TargetPlayer))
					{
						Cast<IDamageHandler>(TargetPlayer)->ChangeObjectHealth(DamageToDeal * -1);
					}
					
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
						if(!ShooterPlayer->bIsSnailCollectorAvailable)
						{
							ShooterPlayer->SnailCollectorKillRegistry++;
							if(ShooterPlayer->SnailCollectorKillRegistry >= GetGameState<ASNAILLGameState>()->SnailCollectorKillRequirement)
							{
								ShooterPlayer->bIsSnailCollectorAvailable = true;
								ShooterPlayer->Client_DisplaySnailCollectorAvailability();
							}
						}
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
		SpawnedPlayer->DisplayBasicUI();
		SpawnedPlayer->SelectDefWeaponTMP();
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

void ASNAILLGameMode::SetKillerDataForEnemy(APlayerCharacter* Player, FString DamageDealerName)
{
	Player->GetPlayerState<ASNAILLPlayerState>()->KillerName = DamageDealerName;
	Player->GetPlayerState<ASNAILLPlayerState>()->ForceNetUpdate();
}

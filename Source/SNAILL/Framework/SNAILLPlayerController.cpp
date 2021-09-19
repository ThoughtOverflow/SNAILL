// Fill out your copyright notice in the Description page of Project Settings.
         
         
#include "SNAILLPlayerController.h"

#include "SNAILLPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "SNAILL/Framework/SNAILLGameMode.h"


ASNAILLPlayerController::ASNAILLPlayerController()
         {
						
         }
         
         void ASNAILLPlayerController::ToggleBasicUI(bool bIsVisible)
         {
         	if (!PlayerBasicUI) return;
         	if (!PlayerBasicUIWidget)
         	{
         		PlayerBasicUIWidget = CreateWidget<class UPlayerDefaultHUD>(GetWorld(), PlayerBasicUI);
         	}
         	if (IsLocalController())
         	{
         		UE_LOG(LogTemp, Warning, TEXT("WHATISGOINGON?"));
         		if (bIsVisible && !PlayerBasicUIWidget->IsVisible())
         		{
         			PlayerBasicUIWidget->AddToViewport();
         		}
         		else
         		{
         			PlayerBasicUIWidget->RemoveFromParent();
         		}
         	}else
         	{
         		UE_LOG(LogTemp, Warning, TEXT("HELLO????"));
         	}
         }
         
         void ASNAILLPlayerController::ToggleTeamSelectionScreen(bool bShow)
         {
         	if (!PlayerSelection) return;
         	if (!PlayerSelectionWidget)
         	{
         		PlayerSelectionWidget = CreateWidget<UTeamSelectionWidget>(GetWorld(), PlayerSelection);
         	}
         	if (IsLocalController())
         	{
         		UE_LOG(LogTemp, Warning, TEXT("Hello There"));
         		if (bShow)
         		{
         			PlayerSelectionWidget->AddToViewport();
         			bShowMouseCursor = true;
         			SetInputMode(FInputModeUIOnly());
         		}
         		else
         		{
         			PlayerSelectionWidget->RemoveFromParent();
         			bShowMouseCursor = false;
         			SetInputMode(FInputModeGameOnly());
         		}
	}
}

void ASNAILLPlayerController::TogglePlayerDeathScreen(bool bShow)
{
	if (!PlayerDeath) return;
	if (!PlayerDeathWidget)
	{
		PlayerDeathWidget = CreateWidget<class UPlayerDeathWidget>(GetWorld(), PlayerDeath);
	}
	if (IsLocalController())
	{
		if (bShow)
		{
			PlayerDeathWidget->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
		else
		{
			PlayerDeathWidget->RemoveFromParent();
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
	}
}

void ASNAILLPlayerController::ShowGameEndScreen(bool bIsWinningTeam)
{
	Client_DisplayEndGameWidget(bIsWinningTeam);
}

void ASNAILLPlayerController::DoRotationVertical(float val)
{
	AddYawInput(val);
	Client_DoRotationVertical(val);
}

void ASNAILLPlayerController::Client_DoRotationVertical_Implementation(float val)
{
	AddYawInput(val);
}

void ASNAILLPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//TEMPORARY!!!

	if(!HasAuthority())
	{
		Server_InitPossess();
	}else
	{
		InitPossess();
	}
	ToggleTeamSelectionScreen(true);
}

void ASNAILLPlayerController::InitPossess()
{
	if(InitPossessActor)
	{
		TArray<AActor*> Inits;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), InitPossessActor, Inits);
		if(APawn* ToPossess = Cast<APawn>(Inits[0]))
		{
			Possess(ToPossess);
		}
	}
}

void ASNAILLPlayerController::SetPlayerName_Implementation(const FString& PlayerName)
{
	if(HasAuthority())
	{
		GetPlayerState<ASNAILLPlayerState>()->PlayerName = PlayerName;
		GetPlayerState<ASNAILLPlayerState>()->ForceNetUpdate();
	}
	
}

void ASNAILLPlayerController::Client_DisplayEndGameWidget_Implementation(bool bIsWinningTeam)
{
	if (!GameEnd) return;
	if (!GameEndWidget)
	{
		GameEndWidget = CreateWidget<class UGameEndWidget>(GetWorld(), GameEnd);
	}
	if (IsLocalController())
	{
		if (bIsWinningTeam)
		{
			GameEndWidget->AddToViewport(100);
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
			GameEndWidget->bIsWinningTeam = true;
			GameEndWidget->RefreshWidget();
		}
		else
		{
			GameEndWidget->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
			GameEndWidget->bIsWinningTeam = false;
			GameEndWidget->RefreshWidget();
		}
	}
}

void ASNAILLPlayerController::RespawnPlayer_Implementation()
{
	Cast<ASNAILLGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->Respawn(this);
}

void ASNAILLPlayerController::SelectTeam_Implementation(EGameTeams Team, ASNAILLPlayerController* PlayerController)
{
	Cast<ASNAILLGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->JoinTeam(Team, PlayerController);
}


void ASNAILLPlayerController::Server_InitPossess_Implementation()
{
	InitPossess();
}

bool ASNAILLPlayerController::Server_InitPossess_Validate()
{
	return true;
}

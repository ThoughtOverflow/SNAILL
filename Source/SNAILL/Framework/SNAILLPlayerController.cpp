// Fill out your copyright notice in the Description page of Project Settings.
         
         
#include "SNAILLPlayerController.h"
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
         		if (bIsVisible)
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
         		}
         		else
         		{
         			PlayerSelectionWidget->RemoveFromParent();
         		}
	}
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

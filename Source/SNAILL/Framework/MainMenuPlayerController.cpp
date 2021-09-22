// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainMenuPlayerController::BeginPlay()
{
	if (!MainMenu) return;
	if (!MainMenuWidget)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenu);
	}
	if (IsLocalController())
	{
		MainMenuWidget->AddToViewport();
	}

		
}

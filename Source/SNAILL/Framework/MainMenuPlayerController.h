// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SNAILL_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> MainMenu;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
		UUserWidget* MainMenuWidget;


protected:

	virtual void BeginPlay() override;
	
};

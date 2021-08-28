// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SNAILL/Gameplay/Enums/EGameTeams.h"
#include "SNAILL/UMG/UserWidgets/PlayerDefaultHUD.h"
#include "SNAILL/UMG/UserWidgets/TeamSelectionWidget.h"
#include "SNAILLPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SNAILL_API ASNAILLPlayerController : public APlayerController
{
	GENERATED_BODY()

	public:

	ASNAILLPlayerController();

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<APawn> InitPossessActor;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerDefaultHUD> PlayerBasicUI;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UPlayerDefaultHUD* PlayerBasicUIWidget;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTeamSelectionWidget> PlayerSelection;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UTeamSelectionWidget* PlayerSelectionWidget;

	UFUNCTION(BlueprintCallable)
		void ToggleBasicUI(bool bIsVisible);
	UFUNCTION(BlueprintCallable)
		void ToggleTeamSelectionScreen(bool bShow);

protected:

	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_InitPossess();
	UFUNCTION()
		void InitPossess();
		
	UFUNCTION(BlueprintCallable, Server, Reliable)
		void SelectTeam(EGameTeams Team, ASNAILLPlayerController* PlayerController);
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SNAILL/Gameplay/Enums/EGameTeams.h"
#include "SNAILL/UMG/UserWidgets/PlayerDefaultHUD.h"
#include "SNAILL/UMG/UserWidgets/TeamSelectionWidget.h"
#include "SNAILL/UMG/UserWidgets/PlayerDeathWidget.h"
#include "SNAILL/UMG/UserWidgets/GameEndWidget.h"
#include "SNAILL/UMG/UserWidgets/PopupWidget.h"

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
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerDeathWidget> PlayerDeath;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UPlayerDeathWidget* PlayerDeathWidget;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameEndWidget> GameEnd;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UGameEndWidget* GameEndWidget;

	UFUNCTION(BlueprintCallable)
		void ToggleBasicUI(bool bIsVisible);
	UFUNCTION(BlueprintCallable)
		void ToggleTeamSelectionScreen(bool bShow);
	UFUNCTION(BlueprintCallable)
		void TogglePlayerDeathScreen(bool bShow);
	UFUNCTION(BlueprintCallable)
		void ShowGameEndScreen(bool bIsWinningTeam);
	UFUNCTION(Client, Reliable)
	void Client_DisplayEndGameWidget(bool bIsWinningTeam);

	UFUNCTION()
		void DoRotationVertical(float val);
	UFUNCTION(Client, Reliable)
		void Client_DoRotationVertical(float val);
	UFUNCTION(Client, Reliable)
		void Client_RefreshPlayerAmmoCount(float clipAmmo, float totalAmmo);



	UFUNCTION(BlueprintCallable)
		void DoTheThing(bool bDo);

	UFUNCTION(Client, Reliable)
		void DoTheClientThing(bool bDo);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UPopupWidget> Thing;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UPopupWidget* ThingWidget;


	
	

protected:

	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_InitPossess();
	UFUNCTION()
		void InitPossess();
		
	UFUNCTION(BlueprintCallable, Server, Reliable)
		void SelectTeam(EGameTeams Team, ASNAILLPlayerController* PlayerController);
	UFUNCTION(BlueprintCallable, Server, Reliable)
		void RespawnPlayer();
	UFUNCTION(BlueprintCallable, Server, Reliable)
		void SetPlayerName(const FString& PlayerName);


	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SNAILL/Gameplay/Enums/EGameTeams.h"

#include "GameEndWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNAILL_API UGameEndWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bIsWinningTeam;
	
	UFUNCTION()
		void RefreshWidget();
	UFUNCTION(BlueprintImplementableEvent)
		void OnWidgetRefreshRequestCreated();
	
	
};

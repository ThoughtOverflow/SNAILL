// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerDeathWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNAILL_API UPlayerDeathWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FString KillerName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 currentKills;

	UFUNCTION()
		void RefreshWidget();
	UFUNCTION(BlueprintImplementableEvent)
		void OnWidgetRefreshRequestCreated();
	
};

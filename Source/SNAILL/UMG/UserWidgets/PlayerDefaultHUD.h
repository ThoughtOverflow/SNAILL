// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerDefaultHUD.generated.h"

/**
 * 
 */
UCLASS()
class SNAILL_API UPlayerDefaultHUD : public UUserWidget
{
	GENERATED_BODY()

	public:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	float PlayerHealth;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	float PlayerMaxHealth;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	bool bIsSuperchargeReady;
	
	
	UFUNCTION(BlueprintCallable)
		void RefreshWidget();
	UFUNCTION(BlueprintImplementableEvent)
		void OnWidgetRefreshRequestCreated();
	
};

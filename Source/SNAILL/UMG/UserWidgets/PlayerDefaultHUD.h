// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SNAILL/Enums/ESuperchargeState.h"

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
	ESuperchargeState SuperchargeState;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	float ShieldChargeLevel;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	float ShieldMaxLevel;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	bool bIsShieldBarRed;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	int32 clipAmmo;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	int32 totalAmmo;
	
	UFUNCTION(BlueprintCallable)
		void RefreshWidget();
	UFUNCTION(BlueprintImplementableEvent)
		void OnWidgetRefreshRequestCreated();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void FlashDamageIndicator(float maskValue);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ToggleReloadAlert(bool bOn);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void DisplayBombAvailability();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void DisplayAlertMessage(const FText& AlertMessage);
	
};

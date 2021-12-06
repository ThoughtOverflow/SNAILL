// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerNameTagBase.generated.h"

/**
 * 
 */
UCLASS()
class SNAILL_API UPlayerNameTagBase : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Properties")
		FText PlayerNameToDisplay;

	UFUNCTION(BlueprintImplementableEvent)
		void OnWidgetRefreshRequestCreated();

	UFUNCTION(BlueprintCallable)
		void RefreshWidget();
	
};

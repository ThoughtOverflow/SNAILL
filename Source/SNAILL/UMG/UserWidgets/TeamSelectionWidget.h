// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TeamSelectionWidget.generated.h"

/**
 * 
 */



UCLASS()
class SNAILL_API UTeamSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
		void OnWidgetRefreshRequestCreated();
	UFUNCTION(BlueprintCallable)
		void RefreshWidget();

	
};

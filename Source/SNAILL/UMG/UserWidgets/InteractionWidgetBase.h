// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class SNAILL_API UInteractionWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Widget Display Properties")
		FText InteractableName;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Widget Display Properties")
		FText InteractableAction;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Widget Display Properties")
		float InteractionTime;
	
public:

	UFUNCTION(BlueprintCallable)
		void RefreshWidget();
	UFUNCTION(BlueprintImplementableEvent)
		void OnWidgetRefreshRequestCreated();	
};

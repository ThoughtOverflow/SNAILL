// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNAILL_API UPopupWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	

	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)

	UFUNCTION(BlueprintImplementableEvent)
	void PreRemoval();
	
};

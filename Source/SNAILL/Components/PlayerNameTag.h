// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "PlayerNameTag.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Custom, meta = (BlueprintSpawnableComponent))
class SNAILL_API UPlayerNameTag : public UWidgetComponent
{
	GENERATED_BODY()
	

	public:

	UPlayerNameTag();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FText PlayerName;

	UFUNCTION(BlueprintCallable)
		void SetPlayerName(FText newName);

protected:

	virtual void BeginPlay() override;
	
};

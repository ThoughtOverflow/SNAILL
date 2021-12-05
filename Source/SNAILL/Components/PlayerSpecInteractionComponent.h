// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionComponent.h"
#include "SNAILL/Character/PlayerCharacter.h"


#include "PlayerSpecInteractionComponent.generated.h"

/**
 * 
 */
UCLASS()
class SNAILL_API UPlayerSpecInteractionComponent : public UInteractionComponent
{
	GENERATED_BODY()

public:

	UPlayerSpecInteractionComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	TArray<APlayerCharacter*> AllowedInteractors;


protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	
public:

	virtual void BeginFocus(APawn* Interactor) override;
	
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "SNAILLPlayerState.h"
#include "Net/UnrealNetwork.h"

ASNAILLPlayerState::ASNAILLPlayerState()
{
	PlayerName = "N/A";
	PlayerCurrentKills = 0;
	KillerName = "someone";
}

void ASNAILLPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASNAILLPlayerState, PlayerName);
	DOREPLIFETIME(ASNAILLPlayerState, PlayerCurrentKills);
	DOREPLIFETIME(ASNAILLPlayerState, KillerName);
}

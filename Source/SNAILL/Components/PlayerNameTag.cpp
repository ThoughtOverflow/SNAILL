// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerNameTag.h"

#include "SNAILL/UMG/UserWidgets/PlayerNameTagBase.h"

UPlayerNameTag::UPlayerNameTag()
{
	SetWidgetSpace(EWidgetSpace::World);
	SetDrawSize(FVector2D(400, 100));
	SetDrawAtDesiredSize(true);
	SetActive(true);
	SetHiddenInGame(false);

	PlayerName = FText::FromString("Player");

	
}

void UPlayerNameTag::SetPlayerName(FText newName)
{
	PlayerName = newName;
	UPlayerNameTagBase* BaseWidget = Cast<UPlayerNameTagBase>(GetUserWidgetObject());

	if(BaseWidget)
	{
		BaseWidget->PlayerNameToDisplay = PlayerName;
		BaseWidget->RefreshWidget();
	}
}

void UPlayerNameTag::BeginPlay()
{
	Super::BeginPlay();

	UPlayerNameTagBase* BaseWidget = Cast<UPlayerNameTagBase>(GetUserWidgetObject());

	if(BaseWidget)
	{
		BaseWidget->PlayerNameToDisplay = PlayerName;
		BaseWidget->RefreshWidget();
	}
	
}

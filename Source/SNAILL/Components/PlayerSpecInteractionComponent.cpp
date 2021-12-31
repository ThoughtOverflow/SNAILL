// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSpecInteractionComponent.h"

#include "Net/UnrealNetwork.h"


UPlayerSpecInteractionComponent::UPlayerSpecInteractionComponent()
{
	SetIsReplicatedByDefault(true);
}

void UPlayerSpecInteractionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerSpecInteractionComponent, AllowedInteractors);
	
}

void UPlayerSpecInteractionComponent::BeginFocus(APawn* Interactor)
{

	if(APlayerCharacter* InteractorCharacter = Cast<APlayerCharacter>(Interactor))
	{
		// UE_LOG(LogTemp, Warning, TEXT("Added One: %s, Looking One: %s"), AllowedInteractors[0]->HasAuthority() ? TEXT("YES") : TEXT("NO"), InteractorCharacter->HasAuthority() ? TEXT("YES"): TEXT("NO"));
		// UE_LOG(LogTemp, Warning, TEXT("We have auth: %s, Array Length: %d"), Interactor->HasAuthority() ? TEXT("YES") : TEXT("NO"), Cast<UPlayerSpecInteractionComponent>(InteractorCharacter->InteractionData.LastViewedInteractionComponent)->AllowedInteractors.Num());
		if(AllowedInteractors.Contains(InteractorCharacter))
		{
			
			Super::BeginFocus(Interactor);
		}
	}
}
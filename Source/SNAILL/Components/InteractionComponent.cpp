// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"


#define LOCTEXT_NAMESPACE "Interaction"

UInteractionComponent::UInteractionComponent()
{

	
	InteractableName = LOCTEXT("InteractableName", "Object");
	InteractableAction = LOCTEXT("InteractableAction", "Use");
	InteractionTime = 0.f;
	InteractionDistance = 2.f;
	bAllowMultipleInteractors = true;

	SetWidgetSpace(EWidgetSpace::Screen);
	SetDrawSize(FVector2D(400, 600));
	SetDrawAtDesiredSize(true);
	SetActive(true);
	SetHiddenInGame(true);
}

void UInteractionComponent::BeginFocus(APawn* Interactor)
{
	OnBeginFocus.Broadcast(Interactor);

	SetHiddenInGame(false);	
}

void UInteractionComponent::EndFocus(APawn* Interactor)
{
	OnEndFocus.Broadcast(Interactor);

	SetHiddenInGame(true);
}

void UInteractionComponent::BeginInteract(APawn* Interactor)
{

	OnBeginInteract.Broadcast(Interactor);
	
	if (FMath::IsNearlyZero(InteractionTime))
	{
		Interact(Interactor);
	}
}

void UInteractionComponent::EndInteract(APawn* Interactor)
{
	OnEndInteract.Broadcast(Interactor);
}

void UInteractionComponent::Interact(APawn* Interactor)
{
	OnInteract.Broadcast(Interactor);
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	/*UInteractionWidgetBase* BaseWidget = Cast<UInteractionWidgetBase>(GetUserWidgetObject());

	if(BaseWidget)
	{
		BaseWidget->InteractableName = InteractableName;
		BaseWidget->InteractableAction = InteractableAction;
		BaseWidget->InteractionTime = InteractionTime;

		BaseWidget->RefreshWidget();
	}*/
	
}

#undef LOCTEXT_NAMESPACE
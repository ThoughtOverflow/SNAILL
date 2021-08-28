// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InteractionComponent.generated.h"

/**
* 
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginFocus, APawn*, Interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndFocus, APawn*, Interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginInteract, APawn*, Interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndInteract, APawn*, Interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, APawn*, Interactor);

UCLASS(ClassGroup = Custom, meta = (BlueprintSpawnableComponent))
class SNAILL_API UInteractionComponent : public UWidgetComponent
{
	GENERATED_BODY()

	public:

	UInteractionComponent();

	UFUNCTION()
		void BeginFocus(APawn* Interactor);
	UFUNCTION()
		void EndFocus(APawn* Interactor);
	UFUNCTION()
		void BeginInteract(APawn* Interactor);
	UFUNCTION()
		void EndInteract(APawn* Interactor);
	UFUNCTION()
		void Interact(APawn* Interactor);


	protected:

	virtual void BeginPlay() override;
	
	public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget Display Properties")
	FText InteractableName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget Display Properties")
	FText InteractableAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget Display Properties")
	float InteractionTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget Display Properties")
	float InteractionDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget Display Properties")
	bool bAllowMultipleInteractors;

	//DELEGATE BINDINGS:

	UPROPERTY(BlueprintAssignable)
	FOnBeginFocus OnBeginFocus;
	UPROPERTY(BlueprintAssignable)
	FOnEndFocus OnEndFocus;
	UPROPERTY(BlueprintAssignable)
	FOnBeginInteract OnBeginInteract;
	UPROPERTY(BlueprintAssignable)
	FOnEndInteract OnEndInteract;
	UPROPERTY(BlueprintAssignable)
	FOnInteract OnInteract;
	
	//------------------
	
};

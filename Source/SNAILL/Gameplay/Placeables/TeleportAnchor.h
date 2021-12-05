// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "SNAILL/Components/InteractionComponent.h"
#include "SNAILL/Components/PlayerSpecInteractionComponent.h"

#include "TeleportAnchor.generated.h"

class ATeleporterCharacter;

UCLASS()
class SNAILL_API ATeleportAnchor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATeleportAnchor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anchor Properties")
		ATeleporterCharacter* OwningPlayer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anchor Properties")
		UStaticMeshComponent* AnchorMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anchor Properties")
		USphereComponent* AnchorBase;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anchor Properties")
		UPlayerSpecInteractionComponent* InteractionComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	UFUNCTION()
		void PickupAnchor(APawn* Interactor);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};

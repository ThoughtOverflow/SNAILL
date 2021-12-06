// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "SNAILL/Gameplay/Placeables/TeleportAnchor.h"

#include "TeleporterCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SNAILL_API ATeleporterCharacter : public APlayerCharacter
{
	GENERATED_BODY()

public:

	ATeleporterCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Teleport Properties")
		TSubclassOf<ATeleportAnchor> TeleportAnchorClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Teleport Properties", Replicated)
		ATeleportAnchor* SpawnedAnchor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Teleport Properties", Replicated)
		bool bAnchorAvailable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Teleport Properties", Replicated)
	float TeleportTimerCooldown;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Teleport Properties", Replicated)
	float TeleportPlacementCooldown;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Teleport Properties", ReplicatedUsing = OnRep_CanTeleport)
	bool bCanTeleport;

	UPROPERTY(BlueprintReadOnly, Replicated)
	FTimerHandle TeleportTimerHandle;
	UPROPERTY(BlueprintReadOnly, Replicated)
	FTimerHandle TeleportPlacementHandle;

protected:

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
		bool PlaceAnchor();
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void Server_TryPlaceAnchor();
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void Server_TeleportToAnchor();


public:

	UFUNCTION(BlueprintCallable)
		bool TeleportToAnchor();

	UFUNCTION(BlueprintCallable)
		void TryPlaceAnchor();
	UFUNCTION(BlueprintCallable)
		void UseSpecialAbility();

	UFUNCTION()
		void TeleportTimerHit();
	UFUNCTION()
		void TeleportPlacementTimerHit();
	UFUNCTION(Server, Reliable)
		void AnchorPickedUp();

	UFUNCTION(BlueprintImplementableEvent)
		void TeleportFX();

	UFUNCTION()
		void OnRep_CanTeleport();
		
	
};

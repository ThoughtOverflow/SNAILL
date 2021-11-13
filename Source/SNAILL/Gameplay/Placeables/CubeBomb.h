// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <Components/SphereComponent.h>

#include "GameFramework/Actor.h"
#include "SNAILL/Framework/SNAILLPlayerController.h"

#include "CubeBomb.generated.h"

class UInteractionComponent;

UCLASS()
class SNAILL_API ACubeBomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubeBomb();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
		USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
		UInteractionComponent* InteractionComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
		USphereComponent* TriggerRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
		USphereComponent* InstakillDamageRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
		USphereComponent* DistanceBasedDamageMaxRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
		UStaticMeshComponent* DamageRangeInicatorMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
		UStaticMeshComponent* InstakillRangeIndicatorMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
		float DamageOverDistanceMultiplier;
	/** Detonation Timer (In Seconds)*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
		float DetonationTimeCountdown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ASNAILLPlayerController* BombDeployer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		FTimerHandle DetonationTimer;

	//Replication Functions:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	//------------

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void TriggerExplosionEvent();

	UFUNCTION(BlueprintImplementableEvent)
		void Blueprint_BombPlaced();
	UFUNCTION(BlueprintCallable)
		void InitializeExplosive(ASNAILLPlayerController* Deployer);
	
};

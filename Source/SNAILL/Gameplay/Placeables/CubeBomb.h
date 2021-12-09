// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <Components/SphereComponent.h>

#include "GameFramework/Actor.h"
#include "SNAILL/Framework/SNAILLPlayerController.h"
#include "SNAILL/Interfaces/DamageHandler.h"

#include "CubeBomb.generated.h"

class APlayerCharacter;
class UInteractionComponent;

UCLASS()
class SNAILL_API ACubeBomb : public AActor, public IDamageHandler
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties", ReplicatedUsing = OnRep_Triggered)
	
		bool bISTriggered;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
		EGameTeams EnemyTeam;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
		float DamageOverDistanceMultiplier;
	/** Detonation Timer (In Seconds)*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
		float DetonationTimeCountdown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ASNAILLPlayerController* BombDeployer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties", Replicated)
		TArray<APlayerCharacter*> PullablePlayers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties", Replicated)
		float CubeHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties", Replicated)
		float CubeMaxHealth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		FTimerHandle DetonationTimer;

	//Replication Functions:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	//------------

	UFUNCTION()
		void OnTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		void InsidePullRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		void OutsidePullRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable)
		void CheckForInstantTrigger();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnRep_Triggered();

	UFUNCTION(BlueprintCallable)
		void TriggerExplosionEvent();

	UFUNCTION(BlueprintImplementableEvent)
		void Blueprint_BombPlaced();
	UFUNCTION(BlueprintCallable)
		void InitializeExplosive(ASNAILLPlayerController* Deployer);
	UFUNCTION()
		void StartCountdown();

	UFUNCTION()
		void ToggleGravPullForEachPlayer(bool bEnable);

	UFUNCTION(BlueprintCallable)
		void DestroyCube();

	virtual void SetObjectHealth(float newHealth) override;
	virtual void ChangeObjectHealth(float deltaHealth) override;
	
};

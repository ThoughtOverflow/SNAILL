// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNAILL/Gameplay/Projectiles/Projectile.h"
#include "SuperchargedProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SNAILL_API ASuperchargedProjectile : public AProjectile
{
	GENERATED_BODY()

public:


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USphereComponent* AoESphere;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 AoEDamageValue;
	

	
public:

	ASuperchargedProjectile();

	virtual void BeginPlay() override;

	virtual void OnImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnBlock(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	UFUNCTION(BlueprintImplementableEvent)
	void RunParticles(float SphereRadius);
	UFUNCTION(BlueprintImplementableEvent)
	void DetachParticleSystem();

	UFUNCTION(Client, Reliable)
		void Client_RunParticles();

	UFUNCTION()
		void DoAoEDamage();
	
};

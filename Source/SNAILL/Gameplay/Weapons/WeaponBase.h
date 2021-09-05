// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class SNAILL_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Properties", ReplicatedUsing = OnRep_AmmoCount)
		float AmmoCount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Properties", ReplicatedUsing = OnRep_AmmoInOneMag)
		float AmmoInOneMag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Properties")
		FText WeaponName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Properties")
		class TSubclassOf<class AProjectile> WeaponProjectile;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weaopon Properties")
		class TSubclassOf<class AProjectile> WeaponProjectileSpecial;
				
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* WeaponMeshComponent;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
		void OnRep_AmmoCount();
	UFUNCTION()
		void OnRep_AmmoInOneMag();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable)
		virtual void Shoot();
	UFUNCTION(BlueprintCallable)
		virtual void ShootSpecial();
	UFUNCTION(BlueprintImplementableEvent)
		void SpawnMuzzleParticle();
	
};

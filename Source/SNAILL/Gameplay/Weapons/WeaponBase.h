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
		float TotalAmmo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Properties", ReplicatedUsing = OnRep_AmmoInOneMag)
		float CurrentClipAmmo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Properties", Replicated)
		float TotalAmmoCapacity;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Properties", Replicated)
		float ClipCapacity;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Properties")
		FText WeaponName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Properties")
		class TSubclassOf<class AProjectile> WeaponProjectile;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weaopon Properties")
		class TSubclassOf<class AProjectile> WeaponProjectileSpecial;
				
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* WeaponMeshComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
		FTimerHandle ShootingTimer;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
		FTimerHandle ReloadTimer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Weapon Properties")
		int32 fireRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Weapon Properties")
		float reloadTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
		bool bCanWeaponShoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Properties", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
		float reloadAlertPercentage;
	
	UFUNCTION()
		void OnRep_AmmoCount();
	UFUNCTION()
		void OnRep_AmmoInOneMag();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Unreliable)
		void Multicast_SpawnMuzzleParticle();
	UFUNCTION()
		virtual void Shoot();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
		float fireTime;

	UPROPERTY(BlueprintReadOnly, Replicated)
		float ammoDiff;

	UPROPERTY(BlueprintReadOnly)
		float ammoAlertTreshold;
	
	UFUNCTION()
		void ReloadTimerHit();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		bool BeginShooting();

	UFUNCTION(BlueprintCallable)
		void EndShooting();
	
	UFUNCTION(BlueprintCallable)
		virtual void ShootSpecial();
	UFUNCTION(BlueprintImplementableEvent)
		void SpawnMuzzleParticle();

	UFUNCTION(BlueprintCallable)
		bool TryReload();
	
};

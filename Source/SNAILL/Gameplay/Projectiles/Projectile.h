// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SNAILL/Gameplay/Weapons/WeaponBase.h"

#include "Projectile.generated.h"

UCLASS()
class SNAILL_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Properties")
		UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Properties")
		UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Properties")
		USphereComponent* SphereCollision;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Properties")
		float ProjectileSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile Properties")
		AWeaponBase* WeaponBase;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile Properties")
		bool bCanDamageAllies;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile Properties")
		int32 projectileDamage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void OnBlock(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};

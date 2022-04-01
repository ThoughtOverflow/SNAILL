// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Throwable.generated.h"


class APlayerCharacter;

UCLASS()
class SNAILL_API AThrowable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Properties")
		USkeletalMeshComponent* ThrowableMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Properties")
		USphereComponent* ThrowableRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Properties")
		USphereComponent* ThrowableRotator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Properties")
	USphereComponent* ThrowableCollider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Properties")
		bool bRegisterCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Properties")
		APlayerCharacter* OwningCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Properties")
		UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Properties")
		float ThrowSpeed;
	UPROPERTY(BlueprintReadWrite, Replicated)
		bool bRotateMesh;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnMeshCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void ShootThrowable();
	UFUNCTION(BlueprintCallable)
		virtual void ThrowableImpacted(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
		void TriggerResetAnimation();

};

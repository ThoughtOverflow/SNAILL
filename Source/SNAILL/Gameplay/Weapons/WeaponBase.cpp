// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "SNAILL/Character/PlayerCharacter.h"
#include "SNAILL/Gameplay/Projectiles/Projectile.h"
#include "DrawDebugHelpers.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
    AActor::SetReplicateMovement(true);
	WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMeshComponent"));
	SetRootComponent(WeaponMeshComponent);

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AWeaponBase, AmmoCount);
	DOREPLIFETIME(AWeaponBase, AmmoInOneMag);
}

void AWeaponBase::OnRep_AmmoCount()
{
}

void AWeaponBase::OnRep_AmmoInOneMag()
{
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::Shoot()
{
	SpawnMuzzleParticle();
	if(GetOwner()->HasAuthority())
	{
		if(WeaponProjectile)
		{
			FVector Forward;
			FRotator Rot;
			//Cast<APlayerCharacter>(GetOwner())->GetActorEyesViewPoint(Forward, Rot);
			FHitResult HitResult;
			//FVector End = Forward + (Rot.Vector() * 1000.f);
			Forward = Cast<APlayerCharacter>(GetOwner())->PlayerCamera->GetComponentLocation();
			FVector End = Forward + (Cast<APlayerCharacter>(GetOwner())->GetController()->GetControlRotation().Vector() * 30000.f);
				GetWorld()->LineTraceSingleByChannel(HitResult, Forward, End, ECC_Visibility);
				//DrawDebugLine(GetWorld(), Forward, End, FColor(0, 255, 0), false, 10, 0, 1.5);
				FVector HitPoint = HitResult.GetActor() ? HitResult.ImpactPoint : End;
				FRotator RotationTowardsHit = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitPoint);
				//DrawDebugLine(GetWorld(), GetActorLocation(), HitPoint, FColor::Purple, false, 5, 0, 1);
				FActorSpawnParameters Parameters;
				Parameters.Instigator = Cast<APawn>(GetOwner());
				AProjectile* P = GetWorld()->SpawnActor<AProjectile>(WeaponProjectile, GetActorLocation(), RotationTowardsHit, Parameters);
				P->WeaponBase = this;
				//UE_LOG(LogTemp, Warning, TEXT("SHOOTING SHOOTING ROT: %s"), *HitPoint.ToString());
		}
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("NOAUTH"));
	}

}

void AWeaponBase::ShootSpecial()
{
	SpawnMuzzleParticle();
	if(GetOwner()->HasAuthority())
	{
		if(WeaponProjectileSpecial)
		{
			FVector Forward;
			FRotator Rot;
			//Cast<APlayerCharacter>(GetOwner())->GetActorEyesViewPoint(Forward, Rot);
			FHitResult HitResult;
			//FVector End = Forward + (Rot.Vector() * 1000.f);
			Forward = Cast<APlayerCharacter>(GetOwner())->PlayerCamera->GetComponentLocation();
			FVector End = Forward + (Cast<APlayerCharacter>(GetOwner())->GetController()->GetControlRotation().Vector() * 30000.f);
			GetWorld()->LineTraceSingleByChannel(HitResult, Forward, End, ECC_Visibility);
			//DrawDebugLine(GetWorld(), Forward, End, FColor(0, 255, 0), false, 10, 0, 1.5);
			FVector HitPoint = HitResult.GetActor() ? HitResult.ImpactPoint : End;
			FRotator RotationTowardsHit = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitPoint);
			//DrawDebugLine(GetWorld(), GetActorLocation(), HitPoint, FColor::Purple, false, 5, 0, 1);
			FActorSpawnParameters Parameters;
			Parameters.Instigator = Cast<APawn>(GetOwner());
			AProjectile* P = GetWorld()->SpawnActor<AProjectile>(WeaponProjectileSpecial, GetActorLocation(), RotationTowardsHit, Parameters);
			P->WeaponBase = this;
			//UE_LOG(LogTemp, Warning, TEXT("SHOOTING SHOOTING ROT: %s"), *HitPoint.ToString());	
		}
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("NOAUTH"));
	}
}


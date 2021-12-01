// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "SNAILL/Character/PlayerCharacter.h"
#include "SNAILL/Gameplay/Projectiles/Projectile.h"
#include "DrawDebugHelpers.h"
#include "SNAILL/Framework/SNAILLGameState.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
    AActor::SetReplicateMovement(true);
	WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMeshComponent"));
	SetRootComponent(WeaponMeshComponent);

	TotalAmmoCapacity = 500;
	ClipCapacity = 30;
	fireRate = 450;
	fireTime = -1;

	bCanWeaponShoot = true;
	bPreReloadFireState = bCanWeaponShoot;
	reloadTime = 2.f;
	ammoDiff = 0.f;

	reloadAlertPercentage = 0.1f;

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		CurrentClipAmmo = ClipCapacity;
		TotalAmmo = TotalAmmoCapacity-ClipCapacity;
		OnRep_AmmoCount();
		OnRep_AmmoInOneMag();
		UE_LOG(LogTemp, Warning, TEXT("Set Default ammo variables for weapon"));
	}
	ammoAlertTreshold = ClipCapacity * reloadAlertPercentage;
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AWeaponBase, TotalAmmo);
	DOREPLIFETIME(AWeaponBase, CurrentClipAmmo);
	DOREPLIFETIME(AWeaponBase, fireRate);
	DOREPLIFETIME(AWeaponBase, fireTime);
	DOREPLIFETIME(AWeaponBase, ShootingTimer);
	DOREPLIFETIME(AWeaponBase, TotalAmmoCapacity);
	DOREPLIFETIME(AWeaponBase, ClipCapacity);
	DOREPLIFETIME(AWeaponBase, bCanWeaponShoot);
	DOREPLIFETIME(AWeaponBase, bPreReloadFireState);
	DOREPLIFETIME(AWeaponBase, reloadTime);
	DOREPLIFETIME(AWeaponBase, ReloadTimer);
	DOREPLIFETIME(AWeaponBase, ammoDiff);
}

void AWeaponBase::OnRep_AmmoCount()
{
	if(APlayerCharacter* OwningCharacter = Cast<APlayerCharacter>(GetOwner()))
	{
		if(ASNAILLPlayerController* C = OwningCharacter->TryGetPlayerController())
		{
			C->Client_RefreshPlayerAmmoCount(CurrentClipAmmo, TotalAmmo);
		}
	}
}

void AWeaponBase::OnRep_AmmoInOneMag()
{
	if(APlayerCharacter* OwningCharacter = Cast<APlayerCharacter>(GetOwner()))
	{
		if(ASNAILLPlayerController* C = OwningCharacter->TryGetPlayerController())
		{
			C->Client_RefreshPlayerAmmoCount(CurrentClipAmmo, TotalAmmo);
			if(CurrentClipAmmo <= ammoAlertTreshold && TotalAmmo >= ammoAlertTreshold)
			{
				if(C->PlayerBasicUIWidget)
				{
					C->PlayerBasicUIWidget->ToggleReloadAlert(true);
				}
			}else
			{
				if(C->PlayerBasicUIWidget)
				{
					C->PlayerBasicUIWidget->ToggleReloadAlert(false);
				}
			}
		}
	}
}

void AWeaponBase::ReinitWeapon()
{
	if(HasAuthority())
	{
		CurrentClipAmmo = ClipCapacity;
		TotalAmmo = TotalAmmoCapacity-ClipCapacity;
		OnRep_AmmoCount();
		OnRep_AmmoInOneMag();
		UE_LOG(LogTemp, Warning, TEXT("Reset default ammo variables for weapon."));
	}
	ammoAlertTreshold = ClipCapacity * reloadAlertPercentage;
}

void AWeaponBase::Multicast_SpawnMuzzleParticle_Implementation()
{
	SpawnMuzzleParticle();
}


// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AWeaponBase::BeginShooting()
{
	if(HasAuthority())
	{
		if(CurrentClipAmmo >= 1)
		{
			fireTime = 1 / (float(fireRate) / 60);
			UE_LOG(LogTemp, Warning, TEXT("Fire Time: %f"), fireTime);
			if(fireTime != -1)
			{
				GetWorldTimerManager().SetTimer(ShootingTimer, this, &AWeaponBase::Shoot, fireTime, true, 0.f);
			}
			return true;
		}
	}
	return false;
}

void AWeaponBase::EndShooting()
{
	if(HasAuthority())
	{
		GetWorldTimerManager().ClearTimer(ShootingTimer);
	}
}

void AWeaponBase::Shoot()
{
	if(HasAuthority() && GetOwner()->HasAuthority())
	{
		if(CurrentClipAmmo >= 1 && bCanWeaponShoot)
		{
			//SpawnMuzzleParticle();
			Multicast_SpawnMuzzleParticle();
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

				CurrentClipAmmo--;
				OnRep_AmmoInOneMag();
				UE_LOG(LogTemp, Warning, TEXT("Current Clip Ammo: %f"), CurrentClipAmmo);
			}
		}else
		{
			EndShooting();
		}
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("NOAUTH"));
	}

}

void AWeaponBase::ShootSpecial()
{
	if(GetOwner()->HasAuthority())
	{
		//SpawnMuzzleParticle();
		Multicast_SpawnMuzzleParticle();
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

bool AWeaponBase::TryReload()
{
	if(HasAuthority() && GetOwner()->HasAuthority())
	{
		if(CurrentClipAmmo != ClipCapacity)
		{
			ammoDiff = ClipCapacity - CurrentClipAmmo;
			if(TotalAmmo > 0)
			{
				bPreReloadFireState = bCanWeaponShoot;
				bCanWeaponShoot = false;
				GetWorldTimerManager().SetTimer(ReloadTimer, this, &AWeaponBase::ReloadTimerHit, reloadTime, false);
				Cast<APlayerCharacter>(GetOwner())->bIsReloading = true;
				return true;
			}
		}
	}
	return false;
}

void AWeaponBase::ReloadTimerHit()
{
	if(TotalAmmo > ammoDiff)
	{
		CurrentClipAmmo = ClipCapacity;
		TotalAmmo -= ammoDiff;
	}else
	{
		CurrentClipAmmo += TotalAmmo;
		TotalAmmo = 0;
	}
	bCanWeaponShoot = bPreReloadFireState;
	OnRep_AmmoCount();
	OnRep_AmmoInOneMag();
	if(APlayerCharacter* OwningPlayer = Cast<APlayerCharacter>(GetOwner()))
	{
		OwningPlayer->bIsReloading = false;
	}
	
}


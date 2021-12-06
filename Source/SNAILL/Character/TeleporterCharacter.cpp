// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleporterCharacter.h"
#include "Net/UnrealNetwork.h"

ATeleporterCharacter::ATeleporterCharacter()
{
	bAnchorAvailable = true;
	bCanTeleport = true;
    TeleportPlacementCooldown = 10.f;
    TeleportTimerCooldown = 5.f;
}

void ATeleporterCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATeleporterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ActivateSpecial", IE_Pressed, this, &ATeleporterCharacter::UseSpecialAbility);
	
}

void ATeleporterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATeleporterCharacter, bAnchorAvailable);
	DOREPLIFETIME(ATeleporterCharacter, SpawnedAnchor);
	DOREPLIFETIME(ATeleporterCharacter, bCanTeleport);
	DOREPLIFETIME(ATeleporterCharacter, TeleportPlacementCooldown);
	DOREPLIFETIME(ATeleporterCharacter, TeleportTimerCooldown);
	DOREPLIFETIME(ATeleporterCharacter, TeleportPlacementHandle);
	DOREPLIFETIME(ATeleporterCharacter, TeleportTimerHandle);
	
}

bool ATeleporterCharacter::PlaceAnchor()
{
	if(HasAuthority())
	{
		FHitResult HitResult;
		FVector StartLocation;
		FRotator StartRotation;

		if (GetController() == nullptr || TeleportAnchorClass == nullptr) return false;
	
		GetController()->GetPlayerViewPoint(StartLocation, StartRotation);
	
		FVector EndLocation = StartLocation + StartRotation.Vector() * 1000.f;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
	
		UWorld* World = GetWorld();
		if (World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
		{
			if(HitResult.GetActor() && HitResult.ImpactNormal.Z > 0.9f)
			{
				SpawnedAnchor =  World->SpawnActor<ATeleportAnchor>(TeleportAnchorClass, HitResult.ImpactPoint, FRotator(0.f,0.f,0.f));
				SpawnedAnchor->SetActorRelativeRotation(HitResult.ImpactNormal.Rotation() + FRotator::MakeFromEuler(FVector(0.f,-90.f,0.f)));
				SpawnedAnchor->OwningPlayer = this;
				SpawnedAnchor->InteractionComponent->AllowedInteractors.Add(this);
				// UE_LOG(LogTemp, Warning, TEXT("Added: %s"), *GetName());
				bAnchorAvailable = false;
			}
		}
		return true;	
	}
	return false;
}

void ATeleporterCharacter::Server_TryPlaceAnchor_Implementation()
{
	TryPlaceAnchor();
}

void ATeleporterCharacter::Server_TeleportToAnchor_Implementation()
{
	TeleportToAnchor();
}

bool ATeleporterCharacter::TeleportToAnchor()
{

	if(!HasAuthority())
	{
		Server_TeleportToAnchor();
	}else
	{
		TeleportFX();
		SetActorLocation(SpawnedAnchor->GetActorLocation());
		GetWorldTimerManager().SetTimer(TeleportTimerHandle, this, &ATeleporterCharacter::TeleportTimerHit, TeleportTimerCooldown, false);
		bCanTeleport = false;
		OnRep_CanTeleport();
		return true;
	}
	
	return false;
}

void ATeleporterCharacter::TryPlaceAnchor()
{
	if(!HasAuthority())
	{
		Server_TryPlaceAnchor();
	}

	if(bAnchorAvailable)
	{
		PlaceAnchor();
	}
	
}

void ATeleporterCharacter::UseSpecialAbility()
{
	if(bAnchorAvailable)
	{
		TryPlaceAnchor();
		
	}else if(!bAnchorAvailable && bCanTeleport)
	{
		TeleportToAnchor();
	}
}

void ATeleporterCharacter::TeleportTimerHit()
{
	bCanTeleport = true;
	OnRep_CanTeleport();
	Client_DisplayAlertMessage(FText::FromString("Teleport Ready"));
}

void ATeleporterCharacter::TeleportPlacementTimerHit()
{
	bCanTeleport = true;
	OnRep_CanTeleport();
	bAnchorAvailable = true;
	Client_DisplayAlertMessage(FText::FromString("Teleport Anchor Available"));
}

void ATeleporterCharacter::OnRep_CanTeleport()
{
    if(SpawnedAnchor) {
	SpawnedAnchor->ToggleTeleporterAvailabilty(bCanTeleport);
	}
}

void ATeleporterCharacter::AnchorPickedUp_Implementation()
{
	GetWorldTimerManager().SetTimer(TeleportPlacementHandle, this, &ATeleporterCharacter::TeleportPlacementTimerHit, TeleportPlacementCooldown, false);
	bAnchorAvailable = false;
	bCanTeleport = false;
	OnRep_CanTeleport();
	GetWorldTimerManager().ClearTimer(TeleportTimerHandle);
}

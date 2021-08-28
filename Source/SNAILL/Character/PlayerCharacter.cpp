// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SNAILL/Components/InteractionComponent.h"
#include "SNAILL/Framework/SNAILLGameState.h"

FInteractionData::FInteractionData()
{
	LastViewedInteractionComponent = nullptr;
	bIsInteractHeld = false;
}

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetOwnerNoSee(false);

	PlayerCurrentJumpBoostCount = 0;
	PlayerMaxJumpBoostCount = 1;
	PlayerAirBoostPower = 650.f;
	AirControlWhileBoosted = 0.4f;

	playerMaxHealth = 100.f;
	playerHealth = 50.f;

	bSprayAvailable = false;

	//----------------COMPONENT INITIALIZATION------------

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "head");
	PlayerCamera->SetRelativeRotation(FRotator(0.0f,90.f,-90.f));
	PlayerCamera->bUsePawnControlRotation = true;

	//----------------------------------------------------
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerCurrentJumpBoostCount = 0;
	if(ASNAILLPlayerController* PlayerController = TryGetPlayerController())
	{
		PC = PlayerController;
	}

	//UE_LOG(LogTemp, Warning, TEXT("WUT? - %s - %s"), IsLocallyControlled() ? TEXT("LOCAL") : TEXT("NOTLOCAL"), *GetName());
	
}



// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckInteractable();
}

void APlayerCharacter::MoveForward(float val)
{
	AddMovementInput(GetActorForwardVector(), val);
}

void APlayerCharacter::MoveRight(float val)
{
	AddMovementInput(GetActorRightVector(), val);
}

void APlayerCharacter::TrySpray()
{

		if(!HasAuthority())
		{
			Server_TrySpray();
		}else
		{
			if(bSprayAvailable)
			{
				//do spraying
				EventDoSpray();
				bSprayAvailable = false;
			}
		}
}

void APlayerCharacter::BeginJump()
{
	if(!HasAuthority())
	{
		Server_BeginJump();
	}
	
	if(GetCharacterMovement()->IsFalling())
	{
		if(PlayerCurrentJumpBoostCount<PlayerMaxJumpBoostCount)
		{
			//Apply Jump boost;
			GetCharacterMovement()->AirControl = AirControlWhileBoosted;
			GetCharacterMovement()->Launch(FVector(0.f, 0.f, PlayerAirBoostPower) + GetVelocity());
			PlayerCurrentJumpBoostCount++;
		}
		
	}else
	{
		bPressedJump = true;
		GetCharacterMovement()->AirControl = 0.05;
		PlayerCurrentJumpBoostCount = 0;
	}
}

void APlayerCharacter::EndJump()
{
	bPressedJump = false;
}

void APlayerCharacter::CheckInteractable()
{
	FHitResult HitResult;
	FVector StartLocation;
	FRotator StartRotation;

	if (GetController() == nullptr) return;
	
	GetController()->GetPlayerViewPoint(StartLocation, StartRotation);
	
	FVector EndLocation = StartLocation + StartRotation.Vector() * 1000.f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	UWorld* World = GetWorld();
	if (World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
	{

		if(HitResult.GetActor())
		{
			
			if (UInteractionComponent* IC = Cast<UInteractionComponent>(HitResult.Actor->GetComponentByClass(UInteractionComponent::StaticClass())))
			{
				float Distance = (HitResult.ImpactPoint - StartLocation).Size();

				if (IC != InteractionData.LastViewedInteractionComponent && Distance <= IC->InteractionDistance * 100.f)
				{
					FoundNewInteractable(IC);
				}
				else if (IC && Distance >= IC->InteractionDistance * 100.f)
				{
					NoNewInteractableFound();
				}

				return;
			}
		}
		
	}

	NoNewInteractableFound();
}

void APlayerCharacter::FoundNewInteractable(UInteractionComponent* FoundComponent)
{
	InteractionData.LastViewedInteractionComponent = FoundComponent;
	if(IsLocallyControlled())
	{
		FoundComponent->BeginFocus(this);
	}
}

void APlayerCharacter::NoNewInteractableFound()
{
	if(InteractionData.LastViewedInteractionComponent!=nullptr)
	{
		if(IsLocallyControlled())
		{
			InteractionData.LastViewedInteractionComponent->EndFocus(this);
		}
		InteractionData.LastViewedInteractionComponent = nullptr;
	}
}

void APlayerCharacter::BeginInteract()
{
	if(!HasAuthority())
	{
		Server_BeginInteract();
	}
	if(InteractionData.LastViewedInteractionComponent!=nullptr)
	{
		InteractionData.LastViewedInteractionComponent->BeginInteract(this);
	}
}

void APlayerCharacter::EndInteract()
{
	if(!HasAuthority())
	{
		Server_EndInteract();
	}

	if (InteractionData.LastViewedInteractionComponent != nullptr)
	{
		InteractionData.LastViewedInteractionComponent->EndInteract(this);
	}
}

void APlayerCharacter::BeginShooting()
{

	if(!HasAuthority())
	{
		Server_BeginShooting();
	}else
	{
			
		if(CurrentWeapon)
		{
			CurrentWeapon->Shoot();
		}
	}

}

ASNAILLPlayerController* APlayerCharacter::TryGetPlayerController()
{
	return GetController<ASNAILLPlayerController>();
}

// void APlayerCharacter::DisplayBasicUI()
// {
//
// }

void APlayerCharacter::Server_BeginInteract_Implementation()
{
	BeginInteract();
}

void APlayerCharacter::Server_EndInteract_Implementation()
{
	EndInteract();
}

void APlayerCharacter::Server_BeginJump_Implementation()
{
	BeginJump();
}

void APlayerCharacter::Server_TrySpray_Implementation()
{
	TrySpray();
}

void APlayerCharacter::Server_BeginShooting_Implementation()
{
	BeginShooting();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MouseX", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("MouseY", this, &ACharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::BeginJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::EndJump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::BeginInteract);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &APlayerCharacter::EndInteract);
	PlayerInputComponent->BindAction("Spray", IE_Released, this, &APlayerCharacter::TrySpray);
	PlayerInputComponent->BindAction("TMP", IE_Released, this, &APlayerCharacter::Server_TMP);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::BeginShooting);

}


void APlayerCharacter::SelectWeapon(TSubclassOf<AWeaponBase> WeaponToSelect)
{
	if(HasAuthority())
	{
			if(WeaponToSelect)
			{
				if(CurrentWeapon)
				{
					CurrentWeapon->Destroy();
				}
				CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponToSelect, GetMesh()->GetSocketLocation("hand_l"), GetMesh()->GetSocketRotation("hand_l"));
				CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("hand_l"));
				CurrentWeapon->SetOwner(this);
				FVector Loc;
				FRotator Rot;
				Rot = GetController()->GetControlRotation();
				CurrentWeapon->SetActorRelativeRotation(FRotator(0.f,0.f,0.f));
				//CurrentWeapon->SetActorRelativeLocation(FVector(0.f,0.f,-50.f));
			}
	}
}

void APlayerCharacter::DisplayBasicUI_Implementation()
{
	if(ASNAILLPlayerController* PlayerController = TryGetPlayerController())
	{
		PlayerController->ToggleBasicUI(true);
		if(PlayerController->PlayerBasicUIWidget)
		{
			PlayerController->PlayerBasicUIWidget->PlayerHealth = playerHealth;
			PlayerController->PlayerBasicUIWidget->PlayerMaxHealth = playerMaxHealth;
			PlayerController->PlayerBasicUIWidget->RefreshWidget();
		}
	}
}

void APlayerCharacter::Server_TMP_Implementation()
{
	/*if(ASNAILLGameState* GS = Cast<ASNAILLGameState>(UGameplayStatics::GetGameState(GetWorld())))
	{
		GS->TeamAKillScore++;
		GS->TeamBKillScore++;
	}*/

	SelectWeapon(WeaponToAdd);
	
}


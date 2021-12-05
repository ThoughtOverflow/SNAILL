// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SNAILL/Components/InteractionComponent.h"
#include "Net/UnrealNetwork.h"
#include "SNAILL/Framework/SNAILLGameMode.h"
#include "SNAILL/Framework/SNAILLPlayerState.h"
#include "SNAILL/Gameplay/Placeables/CubeBomb.h"

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
	
	DefaultWalkSpeed = 600;
	RunningSpeedMultiplier = 2.0f;
	PlayerCurrentJumpBoostCount = 0;
	PlayerMaxJumpBoostCount = 2;
	PlayerAirBoostPower = 900.f;
	AirControlWhileBoosted = 0.4f;
	SpeedFactorMultiplier = 1.f;

	playerMaxHealth = 100.f;
	playerHealth = 100.f;
	playerPrevHealth = 100.f;
	SuperchargeDelay = 120;
	ShieldMaxLevel = 100.f; // Shield Max Time = ShleidMaxLevel / 20;
	ShieldBatteryLevel = 50.f; // Shield Time = ShieldBatteryLevel / 20;
	SnailCollectorKillRegistry = 0;

	bSprayAvailable = false;
	bIsPlayerDead = false;
	SuperchargeState = ESuperchargeState::ESS_Discharged;
	PreviousSuperchargeState = ESuperchargeState::ESS_Disabled;
	bIsShieldBarRed = false;
	bCanSprint = true;
	bIsSnailCollectorAvailable = false;
	bEnableGravPull = false;
	
	bIsReloading = false;
	
	//----------------COMPONENT INITIALIZATION------------

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "head");
	//PlayerCamera->SetupAttachment(GetCapsuleComponent());
	PlayerCamera->SetRelativeRotation(FRotator(0.0f,90.f,-90.f));
	PlayerCamera->bUsePawnControlRotation = true;
	PlayerCamera->SetFieldOfView(90);

	GravPushDirection = FVector::ZeroVector;
	GravPushPower = 0.0f;

	//----------------------------------------------------

	//DEBUG:
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerCurrentJumpBoostCount = 0;
	GetCharacterMovement()->MaxWalkSpeed *= SpeedFactorMultiplier;
	if(ASNAILLPlayerController* PlayerController = TryGetPlayerController())
	{
		PC = PlayerController;
	}

	StartSuperchargeTimer();
	if(HasAuthority())
	{
		GetWorldTimerManager().SetTimer(ShieldTimer, this, &APlayerCharacter::ShieldTimerHit, 0.05, true);
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("WUT? - %s - %s"), IsLocallyControlled() ? TEXT("LOCAL") : TEXT("NOTLOCAL"), *GetName());
	
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, playerHealth);
	DOREPLIFETIME(APlayerCharacter, playerMaxHealth);
	DOREPLIFETIME(APlayerCharacter, bIsPlayerDead);
	DOREPLIFETIME(APlayerCharacter, SuperchargeState);
	DOREPLIFETIME(APlayerCharacter, PreviousSuperchargeState);
	DOREPLIFETIME(APlayerCharacter, SuperchargeDelay);
	DOREPLIFETIME(APlayerCharacter, bIsUsingShield);
	DOREPLIFETIME(APlayerCharacter, ShieldBatteryLevel);
	DOREPLIFETIME(APlayerCharacter, ShieldMaxLevel);
	DOREPLIFETIME(APlayerCharacter, bIsShieldBarRed);
	DOREPLIFETIME(APlayerCharacter, SpeedFactorMultiplier);
	DOREPLIFETIME(APlayerCharacter, playerPrevHealth);
	DOREPLIFETIME(APlayerCharacter, bCanSprint);
	DOREPLIFETIME(APlayerCharacter, bEnableGravPull);
	DOREPLIFETIME(APlayerCharacter, bIsSnailCollectorAvailable);
	DOREPLIFETIME(APlayerCharacter, SnailCollectorKillRegistry);
	DOREPLIFETIME(APlayerCharacter, bIsReloading);
	
}

void APlayerCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	if(HasAuthority())
	{
		switch (Cast<ASNAILLGameState>(UGameplayStatics::GetGameState(GetWorld()))->GetPlayerTeam(TryGetPlayerController()))
		{
		case EGameTeams::EGT_TeamA:
			Cast<ASNAILLGameState>(UGameplayStatics::GetGameState(GetWorld()))->ChangeTeamScore(EGameTeams::EGT_TeamB, 1);
			break;
		case EGameTeams::EGT_TeamB:
			Cast<ASNAILLGameState>(UGameplayStatics::GetGameState(GetWorld()))->ChangeTeamScore(EGameTeams::EGT_TeamA, 1);
			break;
		case EGameTeams::EGT_TeamNone:
			break;
						
		}
		Cast<ASNAILLGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->Respawn(Cast<ASNAILLPlayerController>(GetController()));
	}
}

void APlayerCharacter::OnRep_SuperchargeState()
{
	//TODO: UILOGIC FOR SUPERCHARGE;
	if(ASNAILLPlayerController* PlayerController = TryGetPlayerController())
	{
		if(PlayerController->PlayerBasicUIWidget) {
			PlayerController->PlayerBasicUIWidget->SuperchargeState = SuperchargeState;
			PlayerController->PlayerBasicUIWidget->RefreshWidget();
		}
	}
}

void APlayerCharacter::OnRep_ShieldBattery()
{
	if(ASNAILLPlayerController* PlayerController = TryGetPlayerController())
	{
		if(PlayerController->PlayerBasicUIWidget) {
			PlayerController->PlayerBasicUIWidget->ShieldChargeLevel = ShieldBatteryLevel;
			PlayerController->PlayerBasicUIWidget->bIsShieldBarRed = bIsShieldBarRed;
			PlayerController->PlayerBasicUIWidget->RefreshWidget();
		}
	}
}

void APlayerCharacter::OnRep_ShieldMaxBattery()
{
	if(ASNAILLPlayerController* PlayerController = TryGetPlayerController())
	{
		if(PlayerController->PlayerBasicUIWidget) {
			PlayerController->PlayerBasicUIWidget->ShieldMaxLevel = ShieldMaxLevel;
			PlayerController->PlayerBasicUIWidget->RefreshWidget();
		}
	}
}


void APlayerCharacter::OnSuperchargeFinished()
{
	SuperchargeState = ESuperchargeState::ESS_Charged;
	OnRep_SuperchargeState();
}

void APlayerCharacter::StartSuperchargeTimer_Implementation()
{
	if(!bIsUsingShield)
	{
		GetWorldTimerManager().SetTimer(SuperchargeTimer, this, &APlayerCharacter::OnSuperchargeFinished, SuperchargeDelay, false);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CheckInteractable();
	
	//TIck grav pull: WTF

	if(IsLocallyControlled() && bEnableGravPull)
	{
		AddMovementInput(GravPushDirection, GravPushPower);
	}
	
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

void APlayerCharacter::EndInteractFocus()
{
	if(InteractionData.LastViewedInteractionComponent!=nullptr)
	{
		if(IsLocallyControlled())
		{
			UE_LOG(LogTemp, Warning, TEXT("LÓGING"))
			InteractionData.LastViewedInteractionComponent->EndFocus(this);
		}
	}
}


void APlayerCharacter::BeginInteract()
{
	if(!HasAuthority())
	{
		Server_BeginInteract();
	}
	if(InteractionData.LastViewedInteractionComponent!=nullptr && !bIsReloading)
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
			CurrentWeapon->BeginShooting();
		}
	}

}

void APlayerCharacter::EndShooting()
{
	if(!HasAuthority())
	{
		Server_EndShooting();
	}else
	{
			
		if(CurrentWeapon)
		{
			CurrentWeapon->EndShooting();
		}
	}
}

void APlayerCharacter::Reload()
{
	if(!HasAuthority())
	{
		Server_Reload();
	}

	if(CurrentWeapon)
	{
		bool reloadSuccess = CurrentWeapon->TryReload();
		UE_LOG(LogCore, Warning, TEXT("%s"), reloadSuccess ? TEXT("Reload Successful!") : TEXT("Reload Invalid!"))
	}
	
}

void APlayerCharacter::BeginShootingSpecial()
{
	if(!HasAuthority())
	{
		Server_BeginShootingSpecial();
	}else
	{
			
		if(CurrentWeapon && SuperchargeState == ESuperchargeState::ESS_Charged && !bIsUsingShield)
		{
			CurrentWeapon->ShootSpecial();
			SuperchargeState = ESuperchargeState::ESS_Discharged;
			OnRep_SuperchargeState();
			GetWorldTimerManager().ClearTimer(SuperchargeTimer);
			StartSuperchargeTimer();
		}else
		{
			UE_LOG(LogTemp, Warning, TEXT("NOTREADYUIDIOT"));
		}
	}
}

void APlayerCharacter::BeginSprinting()
{
	if(!HasAuthority())
	{
		Server_BeginSprinting();
	}
	if(bCanSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed * RunningSpeedMultiplier * SpeedFactorMultiplier;
	}
}

void APlayerCharacter::EndSprinting()
{
	if(!HasAuthority())
	{
		Server_EndSprinting();
	}
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed * SpeedFactorMultiplier;
}

void APlayerCharacter::Client_BlockSprinting_Implementation()
{
	EndSprinting();
}

void APlayerCharacter::Server_BeginSprinting_Implementation()
{
	BeginSprinting();
}

void APlayerCharacter::Server_EndSprinting_Implementation()
{
	EndSprinting();
}

void APlayerCharacter::EnableShield()
{
	if(!HasAuthority())
	{
		Server_EnableShield();
	}else
	{
		if(!bIsUsingShield && ShieldBatteryLevel/ShieldMaxLevel >= 0.25f)
		{
			TogglePlayerShield(!bIsUsingShield);
			bIsUsingShield = !bIsUsingShield;
			PreviousSuperchargeState = SuperchargeState;
			SuperchargeState = ESuperchargeState::ESS_Disabled;
			OnRep_SuperchargeState();
			GetWorldTimerManager().PauseTimer(SuperchargeTimer);
			if(CurrentWeapon) CurrentWeapon->bCanWeaponShoot = false;
		}
		
	}

}

void APlayerCharacter::PlaceBomb()
{
	
	if(HasAuthority())
	{
		if(CubeBomb==nullptr || !bIsSnailCollectorAvailable) return;
		
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
				ACubeBomb* Bomb =  World->SpawnActor<ACubeBomb>(CubeBomb, HitResult.ImpactPoint, FRotator(0.f,0.f,0.f));
				Bomb->SetActorRelativeRotation(HitResult.ImpactNormal.Rotation() + FRotator::MakeFromEuler(FVector(0.f,-90.f,0.f)));
				Bomb->InitializeExplosive(TryGetPlayerController());
				bIsSnailCollectorAvailable = false;
				SnailCollectorKillRegistry = 0;
			}
		}
		
	}else
	{
		Server_PlaceBomb();
	}
}

void APlayerCharacter::Client_DisplaySnailCollectorAvailability_Implementation()
{
	//TODO: Show that snail collector is available;
	if(ASNAILLPlayerController* PlayerController = TryGetPlayerController())
	{
		if(PlayerController->PlayerBasicUIWidget) {
			PlayerController->PlayerBasicUIWidget->DisplayBombAvailability();
			PlayerController->PlayerBasicUIWidget->RefreshWidget();
		}
	}
}

void APlayerCharacter::Server_PlaceBomb_Implementation()
{
	PlaceBomb();
}

void APlayerCharacter::Client_ChangeWalkSpeed_Implementation(float newWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = newWalkSpeed;
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

void APlayerCharacter::Server_EndShooting_Implementation()
{
	EndShooting();
}


void APlayerCharacter::Server_BeginShootingSpecial_Implementation()
{
	BeginShootingSpecial();
}

void APlayerCharacter::Server_DoRotationVertical_Implementation(float val)
{
	if(HasAuthority())
	{
		ASNAILLPlayerController* CurrentPC = Cast<ASNAILLPlayerController>(GetController());
		
		CurrentPC->DoRotationVertical(val);
		
	}
}

void APlayerCharacter::Server_EnableShield_Implementation()
{
	EnableShield();
}

void APlayerCharacter::Server_Reload_Implementation()
{
	Reload();
}

void APlayerCharacter::SelectDefWeaponTMP()
{
	SelectWeapon(WeaponToAdd);
}

void APlayerCharacter::OnRep_PlayerHealth()
{
	if(ASNAILLPlayerController* PlayerController = TryGetPlayerController())
	{
		if(PlayerController->PlayerBasicUIWidget)
		{
			PlayerController->PlayerBasicUIWidget->PlayerHealth = playerHealth;
			PlayerController->PlayerBasicUIWidget->PlayerMaxHealth = playerMaxHealth;
			// PlayerController->PlayerBasicUIWidget->SuperchargeState = false;
			PlayerController->PlayerBasicUIWidget->RefreshWidget();
			if(playerHealth < playerPrevHealth)
			{
				if(IsLocallyControlled())
				{
					UE_LOG(LogTemp, Error, TEXT("BLINKED HIT WIDGET"));
					BlinkHitWidget();
				}
			}
		}
	}
}

void APlayerCharacter::OnRep_PlayerMaxHealth()
{
	if(ASNAILLPlayerController* PlayerController = TryGetPlayerController())
	{
		if(PlayerController->PlayerBasicUIWidget)
		{
			PlayerController->PlayerBasicUIWidget->PlayerHealth = playerHealth;
			PlayerController->PlayerBasicUIWidget->PlayerMaxHealth = playerMaxHealth;
			PlayerController->PlayerBasicUIWidget->RefreshWidget();
			UE_LOG(LogTemp, Error, TEXT("HP: %f"), playerHealth);
		}
	}
}

void APlayerCharacter::OnRep_IsPlayerDead()
{
	if(bIsPlayerDead)
	{
		GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if(ASNAILLPlayerController* PlayerController = TryGetPlayerController())
		{
			PlayerController->ToggleBasicUI(false);
			PlayerController->TogglePlayerDeathScreen(true);
			if(PlayerController->PlayerDeathWidget)
			{
				PlayerController->PlayerDeathWidget->KillerName = GetPlayerState<ASNAILLPlayerState>()->KillerName;
				PlayerController->PlayerDeathWidget->currentKills = Cast<ASNAILLPlayerState>( GetPlayerState())->PlayerCurrentKills;
				PlayerController->PlayerDeathWidget->RefreshWidget();
				UE_LOG(LogTemp, Warning, TEXT("UPDATED UI"));
			}
				
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MouseX", this, &ACharacter::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("MouseX", this, &APlayerCharacter::Server_DoRotationVertical);
	PlayerInputComponent->BindAxis("MouseY", this, &ACharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::BeginJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::EndJump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::BeginInteract);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &APlayerCharacter::EndInteract);
	PlayerInputComponent->BindAction("Spray", IE_Released, this, &APlayerCharacter::TrySpray);
	PlayerInputComponent->BindAction("TMP", IE_Released, this, &APlayerCharacter::Server_TMP);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::BeginShooting);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::EndShooting);
	PlayerInputComponent->BindAction("Fire_Special", IE_Pressed, this, &APlayerCharacter::BeginShootingSpecial);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::BeginSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::EndSprinting);
	PlayerInputComponent->BindAction("ActivateShield", IE_Pressed, this, &APlayerCharacter::EnableShield);
	PlayerInputComponent->BindAction("ToggleBomb", IE_Pressed, this, &APlayerCharacter::PlaceBomb);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::Reload);

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
				CurrentWeapon->OnRep_AmmoInOneMag(); // Added for server ammo update at the beginning...
				FVector Loc;
				FRotator Rot;
				Rot = GetController()->GetControlRotation();
				CurrentWeapon->SetActorRelativeRotation(FRotator(0.f,0.f,0.f));
				//CurrentWeapon->SetActorRelativeLocation(FVector(0.f,0.f,-50.f));
			}
	}
}

void APlayerCharacter::SetPlayerHealth(float newHealth)
{
	if(HasAuthority())
	{
		float h = FMath::Clamp(newHealth, 0.f, playerMaxHealth);
		playerPrevHealth = playerHealth;
		playerHealth = h;
		OnRep_PlayerHealth();
		if(playerHealth == 0)
		{
			//TODO: Player Death Logic;
			UE_LOG(LogTemp, Error, TEXT("URDEAD"));
			bIsPlayerDead = true;
			OnRep_IsPlayerDead();
			// Cast<ASNAILLPlayerState>(GetPlayerState())->PlayerCurrentKills = 0;
			
		}
	}
}

void APlayerCharacter::ChangePlayerHealth(float deltaHealth)
{
	if(HasAuthority())
	{
		SetPlayerHealth(GetPlayerHealth() + deltaHealth);
	}
}

void APlayerCharacter::ShieldTimerHit()
{
	if(HasAuthority())
	{
		if (bIsUsingShield)
		{
			if(ShieldBatteryLevel<=0)
			{
				TogglePlayerShield(false);
				bIsUsingShield = false;
				SuperchargeState = PreviousSuperchargeState;
				if(PreviousSuperchargeState == ESuperchargeState::ESS_Discharged)
				{
					if(GetWorldTimerManager().TimerExists(SuperchargeTimer))
					{
						GetWorldTimerManager().UnPauseTimer(SuperchargeTimer);
					}else
					{
						StartSuperchargeTimer();
					}
				}
				OnRep_SuperchargeState();

				if(CurrentWeapon) CurrentWeapon->bCanWeaponShoot = true;
				
			}else
			{
				ShieldBatteryLevel--;
				OnRep_ShieldBattery();
			}
			
		
		}else
		{
			if(ShieldBatteryLevel<ShieldMaxLevel)
			{
				if(ShieldBatteryLevel/ShieldMaxLevel >= 0.25f)
				{
					bIsShieldBarRed = false;
				}else
				{
					bIsShieldBarRed = true;
				}
				ShieldBatteryLevel++;
				OnRep_ShieldBattery();
			}
		}	
	}
}

void APlayerCharacter::UpdateGravPush_Implementation(FVector MovementVector, float val)
{
	// if(GetNetMode() == NM_DedicatedServer) return;
	
	if(IsLocallyControlled())
	{
		GravPushDirection = MovementVector;
		GravPushPower = val;
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
			PlayerController->PlayerBasicUIWidget->SuperchargeState = ESuperchargeState::ESS_Discharged;
			PlayerController->PlayerBasicUIWidget->ShieldChargeLevel = ShieldBatteryLevel;
			PlayerController->PlayerBasicUIWidget->ShieldMaxLevel = ShieldMaxLevel;
			PlayerController->PlayerBasicUIWidget->bIsShieldBarRed = bIsShieldBarRed;
			PlayerController->PlayerBasicUIWidget->clipAmmo = 0;
			PlayerController->PlayerBasicUIWidget->totalAmmo = 0;
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

	
	
}


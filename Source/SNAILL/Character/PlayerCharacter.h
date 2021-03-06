// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "SNAILL/Components/PlayerNameTag.h"
#include "SNAILL/Enums/ESuperchargeState.h"
#include "SNAILL/Framework/SNAILLPlayerController.h"
#include "SNAILL/Gameplay/Placeables/CubeBomb.h"
#include "SNAILL/Gameplay/Weapons/WeaponBase.h"
#include "SNAILL/Interfaces/DamageHandler.h"

#include "PlayerCharacter.generated.h"

class UInteractionComponent;

USTRUCT(BlueprintType)
struct FInteractionData
{
	GENERATED_BODY()

public:

	FInteractionData();

	UPROPERTY(BlueprintReadWrite)
	UInteractionComponent* LastViewedInteractionComponent;
	UPROPERTY(BlueprintReadWrite)
	bool bIsInteractHeld;
	
};

UCLASS()
class SNAILL_API APlayerCharacter : public ACharacter, public IDamageHandler
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<AWeaponBase> WeaponToAdd;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCameraComponent* PlayerCamera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UPlayerNameTag* PlayerNameTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 PlayerMaxJumpBoostCount;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	int32 PlayerCurrentJumpBoostCount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin=1.0))
	float PlayerAirBoostPower;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin=0.05, ClampMax=1.0))
	float AirControlWhileBoosted;
	UPROPERTY(BlueprintReadWrite)
	FInteractionData InteractionData;
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	class ASNAILLPlayerController* PC;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool bSprayAvailable;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_IsPlayerDead)
		bool bIsPlayerDead;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated)
		bool bCanSprint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated)
		bool bIsSnailCollectorAvailable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated)
		int32 SnailCollectorKillRegistry;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
		float SpeedFactorMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float DefaultWalkSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float RunningSpeedMultiplier;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	bool bEnableGravPull;
	
	//Weapon Stuff:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons")
		TSubclassOf<AWeaponBase> WeaponClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AWeaponBase* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_SuperchargeState)
		ESuperchargeState SuperchargeState;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	ESuperchargeState PreviousSuperchargeState;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated)
		int32 SuperchargeDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_ShieldBattery)
		float ShieldBatteryLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_ShieldMaxBattery)
		float ShieldMaxLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
		bool bIsUsingShield;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
		bool bIsShieldBarRed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapons")
		TSubclassOf<ACubeBomb> CubeBomb;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated)
		bool bIsReloading;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USoundAttenuation* DefaultSoundAttenuation;

	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void FellOutOfWorld(const UDamageType& dmgType) override;

	UFUNCTION()
		void OnRep_SuperchargeState();
	UFUNCTION(BlueprintCallable)
		void OnRep_ShieldBattery();
	UFUNCTION(BlueprintCallable)
		void OnRep_ShieldMaxBattery();

	UFUNCTION()
		void TrySpray();
	UFUNCTION(BlueprintImplementableEvent)
		void EventDoSpray();
	UFUNCTION()
	void MoveForward(float val);
	UFUNCTION()
		void MoveRight(float val);
	UFUNCTION()
		void BeginJump();
	UFUNCTION()
		void EndJump();
	UFUNCTION()
		void CheckInteractable();
	UFUNCTION()
		void BeginInteract();
	UFUNCTION()
		void EndInteract();
	UFUNCTION()
		void BeginShooting();
	UFUNCTION()
		void EndShooting();
	UFUNCTION()
		void Reload();
	UFUNCTION()
		void BeginShootingSpecial();
	UFUNCTION(Server, Reliable)
		void Server_BeginSprinting();
	UFUNCTION(Server, Reliable)
		void Server_EndSprinting();
	UFUNCTION()
		void EnableShield();
	UFUNCTION(BlueprintCallable)
		void PlaceBomb();
	UFUNCTION(Server, Reliable)
		void Server_PlaceBomb();
	

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Player Properties", ReplicatedUsing = OnRep_PlayerHealth)
	float playerHealth;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_PlayerMaxHealth)
	float playerMaxHealth;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Replicated)
	float playerPrevHealth;
	UPROPERTY(BlueprintReadWrite)
		FTimerHandle SuperchargeTimer;
	UPROPERTY(BlueprintReadWrite)
		FTimerHandle ShieldTimer;
	UFUNCTION()
		void OnSuperchargeFinished();
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void StartSuperchargeTimer();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector GravPushDirection;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float GravPushPower;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginSprinting();
	UFUNCTION()
		void EndSprinting();
	UFUNCTION(Client, Reliable, BlueprintCallable)
		void Client_BlockSprinting();

	UFUNCTION(BlueprintCallable)
	ASNAILLPlayerController* TryGetPlayerController();


	UFUNCTION()
	void FoundNewInteractable(UInteractionComponent* FoundComponent);
	UFUNCTION()
		void NoNewInteractableFound();

	UFUNCTION(BlueprintCallable)
		void EndInteractFocus();

	
	UFUNCTION(Server, Reliable)
		void Server_BeginInteract();
	UFUNCTION(Server, Reliable)
		void Server_EndInteract();
	UFUNCTION(Server, Reliable)
		void Server_BeginJump();
	UFUNCTION(Server, Reliable)
		void Server_TrySpray();
	UFUNCTION(Server, Reliable)
		void Server_BeginShooting();
	UFUNCTION(Server, Reliable)
		void Server_EndShooting();
	UFUNCTION(Server, Reliable)
		void Server_BeginShootingSpecial();
	UFUNCTION(Server, Reliable)
		void Server_EnableShield();
	UFUNCTION(Server, Reliable)
		void Server_Reload();
			
	UFUNCTION(Server, Reliable)
		void Server_DoRotationVertical(float val);

	UFUNCTION()
		void SelectDefWeaponTMP();

	UFUNCTION()
		void OnRep_PlayerHealth();
	UFUNCTION()
		void OnRep_PlayerMaxHealth();
	UFUNCTION()
		void OnRep_IsPlayerDead();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//TEMPORARY:

	UFUNCTION(Server, Reliable)
		void Server_TMP();

	//Weapon Stuff:

	UFUNCTION(BlueprintCallable)
		void SelectWeapon(TSubclassOf<AWeaponBase> WeaponToSelect);
		
	UFUNCTION(Client, Reliable)
		void DisplayBasicUI();
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_ChangeWalkSpeed(float newWalkSpeed);

	UFUNCTION(BlueprintCallable)
		void SetPlayerHealth(float newHealth);
	UFUNCTION(BlueprintCallable)
		void ChangePlayerHealth(float deltaHealth);
	UFUNCTION(BlueprintGetter)
		FORCEINLINE float GetPlayerHealth() {return  playerHealth;};

	UFUNCTION(BlueprintImplementableEvent)
		void BlinkHitWidget();
	UFUNCTION(BlueprintImplementableEvent)
		void TogglePlayerShield(bool bEnable);

	UFUNCTION()
		void ShieldTimerHit();

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void UpdateGravPush(FVector MovementVector, float val);

	UFUNCTION(Client, Reliable)
	void Client_DisplaySnailCollectorAvailability();
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_DisplayAlertMessage(const FText& AlertMessage);
	
	UFUNCTION(BlueprintCallable)
	    void NetPlaySoundAtSocket(FName Socket, USoundBase* Sound);
	UFUNCTION(NetMulticast, Reliable)
		void Multicast_PlaySoundAtSocket(FName Socket, USoundBase* Sound);
	UFUNCTION(Server, Reliable)
		void Server_PlaySoundAtSocket(FName Socket, USoundBase* Sound);

	UFUNCTION(BlueprintCallable)
	virtual void SetObjectHealth(float newHealth) override;
	UFUNCTION(BlueprintCallable)
	virtual void ChangeObjectHealth(float deltaHealth) override;

	UFUNCTION(BlueprintCallable)
		void DebugSuicide();
	UFUNCTION(Server, Reliable)
		void Server_DebugSuicide();
	

};

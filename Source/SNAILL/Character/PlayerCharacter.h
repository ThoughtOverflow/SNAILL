// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "SNAILL/Framework/SNAILLPlayerController.h"
#include "SNAILL/Gameplay/Weapons/WeaponBase.h"

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
class SNAILL_API APlayerCharacter : public ACharacter
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
	int32 PlayerMaxJumpBoostCount;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	int32 PlayerCurrentJumpBoostCount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin=1.0))
	float PlayerAirBoostPower;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin=0.05, ClampMax=1.0))
	float AirControlWhileBoosted;
	UPROPERTY(BlueprintReadWrite)
	FInteractionData InteractionData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Properties")
	float playerHealth;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	float playerMaxHealth;
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
	class ASNAILLPlayerController* PC;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool bSprayAvailable;

	//Weapon Stuff:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons")
		TSubclassOf<AWeaponBase> WeaponClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AWeaponBase* CurrentWeapon;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
		void FoundNewInteractable(UInteractionComponent* FoundComponent);
	UFUNCTION()
		void NoNewInteractableFound();
	UFUNCTION()
		void BeginInteract();
	UFUNCTION()
		void EndInteract();
	UFUNCTION()
		void BeginShooting();
	UFUNCTION(BlueprintCallable)
		ASNAILLPlayerController* TryGetPlayerController();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
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

	

};

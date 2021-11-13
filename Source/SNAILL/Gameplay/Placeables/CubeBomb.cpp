// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeBomb.h"

#include "Kismet/GameplayStatics.h"
#include "SNAILL/Components/InteractionComponent.h"
#include "Net/UnrealNetwork.h"
#include "SNAILL/Character/PlayerCharacter.h"
#include "SNAILL/Framework/SNAILLGameMode.h"
#include "SNAILL/Framework/SNAILLPlayerState.h"

// Sets default values
ACubeBomb::ACubeBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction"));
	TriggerRadius = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerRadius"));
	InstakillDamageRadius = CreateDefaultSubobject<USphereComponent>(TEXT("InstakillDamageRadius"));
	DistanceBasedDamageMaxRadius = CreateDefaultSubobject<USphereComponent>(TEXT("DistanceBasedDamageRadius"));
	DamageRangeInicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DamageIndicator"));
	InstakillRangeIndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DamageIndicator2"));

	SetRootComponent(SkeletalMesh);
	bReplicates = true;
	SetReplicatingMovement(true);
	TriggerRadius->InitSphereRadius(200.f);
	InstakillDamageRadius->InitSphereRadius(500.f);
	DistanceBasedDamageMaxRadius->InitSphereRadius(1500.f);
	InstakillDamageRadius->SetupAttachment(Cast<USceneComponent>(GetRootComponent()));
	DistanceBasedDamageMaxRadius->SetupAttachment(Cast<USceneComponent>(GetRootComponent()));
	DamageRangeInicatorMesh->SetupAttachment(Cast<USceneComponent>(DistanceBasedDamageMaxRadius));
	InstakillRangeIndicatorMesh->SetupAttachment(Cast<USceneComponent>(InstakillDamageRadius));

	DamageOverDistanceMultiplier = 0.08f; //DamageAmount = ((DistanceSphereRadius - InstakillRadius) - (PlayerDistanceFromCenter - InstakillRadius)) * DamageMultiplier;
	DetonationTimeCountdown = 5.f;

}

// Called when the game starts or when spawned
void ACubeBomb::BeginPlay()
{
	Super::BeginPlay();
	Blueprint_BombPlaced();
	
}

void ACubeBomb::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called every frame
void ACubeBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACubeBomb::TriggerExplosionEvent()
{
	if(HasAuthority())
	{
		TArray<AActor*> PlayerActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), PlayerActors);
		for(auto& Player : PlayerActors)
		{
			float distance = GetDistanceTo(Player);
			if(distance <= InstakillDamageRadius->GetScaledSphereRadius())
			{
				//Kill the player;
				APlayerCharacter* Character = Cast<APlayerCharacter>(Player);
				Cast<ASNAILLGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->SetKillerDataForEnemy(Character, BombDeployer->GetPlayerState<ASNAILLPlayerState>()->PlayerName);
				Character->SetPlayerHealth(0);
			
			}else if(distance < DistanceBasedDamageMaxRadius->GetScaledSphereRadius())
			{
				APlayerCharacter* Character = Cast<APlayerCharacter>(Player);
				//Deal damage accordingly;
				float damageToDeal = FMath::Floor((DistanceBasedDamageMaxRadius->GetScaledSphereRadius() - InstakillDamageRadius->GetScaledSphereRadius() - (distance - InstakillDamageRadius->GetScaledSphereRadius())) * DamageOverDistanceMultiplier);
				Cast<ASNAILLGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->SetKillerDataForEnemy(Character, BombDeployer->GetPlayerState<ASNAILLPlayerState>()->PlayerName);
				Character->ChangePlayerHealth(-1.f * damageToDeal);
				UE_LOG(LogTemp, Warning, TEXT("Dealt damage: %f ~ Player Distance: %f ~ GradSphereRad: %f"), damageToDeal, distance, DistanceBasedDamageMaxRadius->GetScaledSphereRadius());
			}
		}
		this->Destroy();
	}
	
}




void ACubeBomb::InitializeExplosive(ASNAILLPlayerController* Deployer)
{
	if(HasAuthority())
	{
		if(Deployer == nullptr)
		{
			UE_LOG(LogController, Error, TEXT("The bomb does not belong to any player - The BombDeployer variable isn't set! SKIPPING!"));
			return;
		}
		BombDeployer = Deployer;
		GetWorldTimerManager().SetTimer(DetonationTimer, this, &ACubeBomb::TriggerExplosionEvent, DetonationTimeCountdown, false);
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeBomb.h"

#include "GameFramework/CharacterMovementComponent.h"
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

	bISTriggered = false;
	bStartPull = false;

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
	TriggerRadius->SetupAttachment(Cast<USceneComponent>(GetRootComponent()));
	InstakillDamageRadius->SetupAttachment(Cast<USceneComponent>(GetRootComponent()));
	DistanceBasedDamageMaxRadius->SetupAttachment(Cast<USceneComponent>(GetRootComponent()));
	DamageRangeInicatorMesh->SetupAttachment(Cast<USceneComponent>(DistanceBasedDamageMaxRadius));
	InstakillRangeIndicatorMesh->SetupAttachment(Cast<USceneComponent>(InstakillDamageRadius));

	DamageOverDistanceMultiplier = 0.08f; //DamageAmount = ((DistanceSphereRadius - InstakillRadius) - (PlayerDistanceFromCenter - InstakillRadius)) * DamageMultiplier;
	DetonationTimeCountdown = 5.f;

	TriggerRadius->SetGenerateOverlapEvents(true);
	InstakillRangeIndicatorMesh->SetVisibility(false);
	DamageRangeInicatorMesh->SetVisibility(false);
	
	EnemyTeam = EGameTeams::EGT_TeamNone;
	
}

// Called when the game starts or when spawned
void ACubeBomb::BeginPlay()
{
	Super::BeginPlay();
	DistanceBasedDamageMaxRadius->OnComponentBeginOverlap.AddDynamic(this, &ACubeBomb::InsidePullRange);
	DistanceBasedDamageMaxRadius->OnComponentEndOverlap.AddDynamic(this, &ACubeBomb::OutsidePullRange);
	TriggerRadius->OnComponentBeginOverlap.AddDynamic(this, &ACubeBomb::OnTrigger);
}

void ACubeBomb::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACubeBomb, bISTriggered);
	DOREPLIFETIME(ACubeBomb, bStartPull);
	DOREPLIFETIME(ACubeBomb, PullablePlayers);
	
}

// Called every frame
void ACubeBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// FVector PullForce = (GetActorLocation() - BombDeployer->GetPawn()->GetActorLocation());
	// PullForce.Normalize();
	// PullForce *= 5000.f;
	// PullForce += FVector(0.f,0.f,100.f);
	//
	// Cast<APlayerCharacter>(Player)->GetCharacterMovement()->Launch(FVector(0.f,0.f,900.f));

	if(HasAuthority())
	{
		if(bStartPull)
		{
			for(auto& Character : PullablePlayers)
			{
				FVector PullForce = (GetActorLocation() - Character->GetActorLocation());
				PullForce.Normalize();
				PullForce *= FVector(1.f,1.f,0.f);
				//PullForce *= 50.f;
				//PullForce += FVector(0.f,0.f,100.f);

				//Cast<APlayerCharacter>(Character)->AddMovementInput(PullForce, 5.f);
				UE_LOG(LogTemp, Warning, TEXT("FCKN VECTOR: %s"), *PullForce.ToString());
				Character->MovePlayerFromServer(PullForce, 0.55f);
				
			}
		}
	}

}

void ACubeBomb::OnRep_Triggered()
{
	Blueprint_BombPlaced();
	InstakillRangeIndicatorMesh->SetVisibility(true);
	DamageRangeInicatorMesh->SetVisibility(true);
}

void ACubeBomb::TriggerExplosionEvent()
{
	if(HasAuthority())
	{
		for(auto& Character : PullablePlayers)
		{
			if(Character)
			{
				Character->bCanSprint = true;
			}
		}
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
				//
				// float damageToDeal = FMath::Floor((DistanceBasedDamageMaxRadius->GetScaledSphereRadius() - InstakillDamageRadius->GetScaledSphereRadius() - (distance - InstakillDamageRadius->GetScaledSphereRadius())) * DamageOverDistanceMultiplier);
				// Cast<ASNAILLGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->SetKillerDataForEnemy(Character, BombDeployer->GetPlayerState<ASNAILLPlayerState>()->PlayerName);
				// Character->ChangePlayerHealth(-1.f * damageToDeal);
				// UE_LOG(LogTemp, Warning, TEXT("Dealt damage: %f ~ Player Distance: %f ~ GradSphereRad: %f"), damageToDeal, distance, DistanceBasedDamageMaxRadius->GetScaledSphereRadius());
			}
			
		}
		bStartPull = false;
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
		EGameTeams AllyTeam = Cast<ASNAILLGameState>(UGameplayStatics::GetGameState(GetWorld()))->GetPlayerTeam(BombDeployer);
		switch (AllyTeam)
		{
		case EGameTeams::EGT_TeamA:
			EnemyTeam = EGameTeams::EGT_TeamB;
			UE_LOG(LogTemp, Warning, TEXT("TeamB"));
			break;
		case EGameTeams::EGT_TeamB:
			EnemyTeam = EGameTeams::EGT_TeamA;
			UE_LOG(LogTemp, Warning, TEXT("TeamA"));
			break;
		case EGameTeams::EGT_TeamNone:
			EnemyTeam = EGameTeams::EGT_TeamNone;
			UE_LOG(LogTemp, Warning, TEXT("TeamNone"));
			break;
		}
		
	}
}

void ACubeBomb::StartCountdown()
{
	GetWorldTimerManager().SetTimer(DetonationTimer, this, &ACubeBomb::TriggerExplosionEvent, DetonationTimeCountdown, false);
	bStartPull = true;
	for(auto& Character : PullablePlayers)
	{
		Character->bCanSprint = false;
		Character->Client_BlockSprinting();
		Character->EndSprinting();
	}
}


void ACubeBomb::OnTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                          int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Num: %d"), PullablePlayers.Num());
		if(EnemyTeam != EGameTeams::EGT_TeamNone && Cast<ASNAILLGameState>(UGameplayStatics::GetGameState(GetWorld()))->GetPlayerTeam(Cast<APlayerCharacter>(OtherActor)->TryGetPlayerController()) == EnemyTeam)
		{
			UE_LOG(LogTemp, Warning, TEXT("SnailCollector triggered!"));
			if(!bISTriggered)
			{
				StartCountdown();
				bISTriggered = true;
				OnRep_Triggered();
			}
			
		}
	}
}

void ACubeBomb::InsidePullRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor);
		if(!PullablePlayers.Contains(Character))
		{
			PullablePlayers.Add(Character);
			//UE_LOG(LogTemp, Warning, TEXT("Num: %d"), PullablePlayers.Num());			
		}
	}
}

void ACubeBomb::OutsidePullRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(HasAuthority())
	{
		APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor);
		if(PullablePlayers.Contains(Character))
		{
			PullablePlayers.Remove(Character);
			Character->bCanSprint = true;
			//UE_LOG(LogTemp, Warning, TEXT("Num: %d"), PullablePlayers.Num());
		}
		
	}
}


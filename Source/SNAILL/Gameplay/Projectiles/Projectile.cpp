// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "Kismet/GameplayStatics.h"
#include "SNAILL/Framework/SNAILLGameMode.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
    SetReplicateMovement(true);
	WeaponBase = nullptr;
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SphereCollider"));
	//BoxCollision->SetCollisionObjectType(ECC_GameTraceChannel1);
	BoxCollision->SetBoxExtent(FVector(2.f));
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnImpact);
	BoxCollision->OnComponentHit.AddDynamic(this, &AProjectile::OnBlock);
	SetRootComponent(BoxCollision);
	BoxCollision->SetGenerateOverlapEvents(true);
	BoxCollision->SetNotifyRigidBodyCollision(true);
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	//ProjectileMesh->SetSimulatePhysics(true);
	ProjectileMovementComponent->InitialSpeed = 4000.0f;
    ProjectileMovementComponent->MaxSpeed = 15000.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = false;
    ProjectileMovementComponent->Bounciness = 0.3f;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	
	bCanDamageAllies = false;
	projectileDamage = 22;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::OnImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

		if(HasAuthority())
		{
			if(WeaponBase && OtherActor != GetInstigator())
			{
					//OtherActor->Destroy();
					//Destroy();
					UE_LOG(LogTemp, Warning, TEXT("Calling Damage Handler"));
					Cast<ASNAILLGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->ProjectileHit(WeaponBase->GetOwner(), OtherActor, projectileDamage, bCanDamageAllies);
					this->Destroy();
			}
		}	 
	
}

void AProjectile::OnBlock(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Block Destroy"));
	this->Destroy();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




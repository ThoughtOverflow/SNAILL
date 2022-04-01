// Fill out your copyright notice in the Description page of Project Settings.


#include "Throwable.h"
#include "SNAILL/Character/PlayerCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AThrowable::AThrowable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bRegisterCollision = false;

	bReplicates = true;
	SetReplicateMovement(true);

	ThrowableRoot = CreateDefaultSubobject<USphereComponent>(TEXT("RootComp"));
	ThrowableRotator = CreateDefaultSubobject<USphereComponent>(TEXT("Rotator"));
	SetRootComponent(ThrowableRoot);
	// ThrowableCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ThrowableCollider"));
	ThrowableMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThrowableMesh"));

	ThrowableRotator->SetupAttachment(GetRootComponent());
	ThrowableMesh->SetupAttachment(ThrowableRotator);
	// ThrowableMesh->OnComponentBeginOverlap.AddDynamic(this, &AThrowable::OnMeshCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	
	ProjectileMovement->InitialSpeed = 0.0f;
	ProjectileMovement->MaxSpeed = 0.0f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->Bounciness = 0.3f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->Velocity = FVector::ZeroVector;
	ThrowSpeed = 50.f;

	bRotateMesh = true;
	
}

// Called when the game starts or when spawned
void AThrowable::BeginPlay()
{
	Super::BeginPlay();
}

void AThrowable::OnMeshCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("FAAAKJÚÚÚ"));
	if(bRegisterCollision)
	{
		ThrowableImpacted(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}


void AThrowable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AThrowable, bRotateMesh);
}

// Called every frame
void AThrowable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(HasAuthority()) {
	
        if(bRotateMesh)
        {
        	// ThrowableRotator->SetWorldRotation(FRotator(-OwningCharacter->GetControlRotation().Pitch, 0.f,0.f));
            ThrowableRotator->AddWorldRotation((FRotator(0.f,70.f,0.f) * DeltaTime));
        }
	
	}
	

}

void AThrowable::ShootThrowable()
{
	bRotateMesh = false;
	ThrowableRotator->SetRelativeRotation(FRotator::ZeroRotator);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	ThrowableRotator->SetWorldRotation(FRotator(OwningCharacter->GetControlRotation().Pitch - 90.f, OwningCharacter->GetControlRotation().Yaw,0.f));
	FVector FireVector = OwningCharacter->GetControlRotation().Vector();
	FireVector.Normalize();
	FireVector *= ThrowSpeed;
	ProjectileMovement->Velocity = FireVector;
	OwningCharacter->bThrowableEquipped = false;
	bRegisterCollision = true;
	TriggerResetAnimation();
	
}

void AThrowable::ThrowableImpacted(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	ProjectileMovement->Velocity = FVector::ZeroVector;
	
}


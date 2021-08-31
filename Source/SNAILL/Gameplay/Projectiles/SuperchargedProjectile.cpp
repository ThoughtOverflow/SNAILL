// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperchargedProjectile.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "SNAILL/Framework/SNAILLGameMode.h"

ASuperchargedProjectile::ASuperchargedProjectile()
{
	bCanDamageAllies = true;
	projectileDamage = 220;
	AoEDamageValue = 69;

	AoESphere = CreateDefaultSubobject<USphereComponent>(TEXT("AOE_SPHERE"));
	AoESphere->SetSphereRadius(250);
	AoESphere->SetupAttachment(GetRootComponent());
	
	
}

void ASuperchargedProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ASuperchargedProjectile::OnImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//RunParticles();

	
	if(HasAuthority())
    		{
    			if(WeaponBase && OtherActor != GetInstigator())
    			{
    				Client_RunParticles();
    				//OtherActor->Destroy();
    				//Destroy();
    				UE_LOG(LogTemp, Warning, TEXT("Calling Damage Handler"));
    				Cast<ASNAILLGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->ProjectileHit(WeaponBase->GetOwner(), OtherActor, projectileDamage, bCanDamageAllies);
    				DoAoEDamage();
    				this->Destroy();
    			}
    		}
}

void ASuperchargedProjectile::OnBlock(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
		RunParticles(AoESphere->GetScaledSphereRadius());
		if(HasAuthority())
		{
			UE_LOG(LogTemp, Warning, TEXT("Block Destroy"));
			DoAoEDamage();
			this->Destroy();
		}	
	
}

void ASuperchargedProjectile::DoAoEDamage()
{
	TArray<AActor*> Actors;
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjTypes;
	TArray<AActor*> ToIgnore;
	ObjTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), AoESphere->GetComponentLocation(), AoESphere->GetScaledSphereRadius(), ObjTypes, nullptr, ToIgnore, Actors);
	DrawDebugSphere(GetWorld(), AoESphere->GetComponentLocation(), AoESphere->GetScaledSphereRadius(), 4, FColor::Purple);
	UE_LOG(LogTemp, Warning, TEXT("Array Count: %d"), Actors.Num());
	for(AActor* AoEActor : Actors)
	{
		if(APlayerCharacter* AoEPlayer = Cast<APlayerCharacter>(AoEActor))
		{
			AoEPlayer->ChangePlayerHealth(AoEDamageValue * -1);
		}
		
	}
	
}

void ASuperchargedProjectile::Client_RunParticles_Implementation()
{
	RunParticles(AoESphere->GetScaledSphereRadius());
}

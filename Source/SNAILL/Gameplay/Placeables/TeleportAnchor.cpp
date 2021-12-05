// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportAnchor.h"
#include "SNAILL/Character/TeleporterCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATeleportAnchor::ATeleportAnchor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicatingMovement(true);

	OwningPlayer = nullptr;
	AnchorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AnchorMesh"));
	AnchorBase = CreateDefaultSubobject<USphereComponent>(TEXT("AnchorBase"));
	InteractionComponent = CreateDefaultSubobject<UPlayerSpecInteractionComponent>(TEXT("AnchorInteractable"));
	SetRootComponent(AnchorBase);
	AnchorMesh->SetupAttachment(GetRootComponent());
	InteractionComponent->SetupAttachment(GetRootComponent());
	
	InteractionComponent->OnInteract.AddDynamic(this, &ATeleportAnchor::PickupAnchor);

	
}

// Called when the game starts or when spawned
void ATeleportAnchor::BeginPlay()
{
	Super::BeginPlay();
	
}



void ATeleportAnchor::PickupAnchor(APawn* Interactor)
{

	if(OwningPlayer && Cast<APlayerCharacter>(Interactor) == OwningPlayer)
	{
		OwningPlayer->AnchorPickedUp();
		this->Destroy();
	}
	
}

// Called every frame
void ATeleportAnchor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



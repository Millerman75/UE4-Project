// Fill out your copyright notice in the Description page of Project Settings.


#include "MasteringWeaponPickupUE4BK.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MasteringUE4BkCharacter.h"
#include "MasteringInventoryUE4BK.h"
#include "MasteringWeaponUE4BK.h"

// Sets default values
AMasteringWeaponPickupUE4BK::AMasteringWeaponPickupUE4BK()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bGenerateOverlapEventsDuringLevelStreaming = true;

	WeaponMeshRenderer = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SetRootComponent(SphereCollision);
	WeaponMeshRenderer->SetupAttachment(SphereCollision);

}

// Called when the game starts or when spawned
void AMasteringWeaponPickupUE4BK::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMasteringWeaponPickupUE4BK::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMasteringWeaponPickupUE4BK::NotifyActorBeginOverlap(AActor* OtherActor)
{

	AMasteringUE4BkCharacter* Player = Cast<AMasteringUE4BkCharacter>(OtherActor);
	if (Player == nullptr)
	{
		return;
	}
	class UMasteringInventoryUE4BK* Inventory = Player->GetInventory();
	Inventory->AddNewWeapon(WeaponPickup, AllocatedAmmo, PowerLevel);
	Inventory->SelectNextAvailableWeapon();

	this->Destroy(); //destroy this temporal weapon pickup class
}


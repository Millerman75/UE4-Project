

// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringWeaponUE4BK.h"
#include "MasteringInventoryUE4BK.h"
#include "MasteringProjectileUE4BK.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/UnrealString.h"

// Sets default values
AMasteringWeaponUE4BK::AMasteringWeaponUE4BK()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DynamicMuzzleLocComp = CreateDefaultSubobject<USceneComponent>(TEXT("Configurable Muzzle Location"));
	FirstPersonGunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Skeletal Mesh Renderer"));

	SetRootComponent(FirstPersonGunMesh);
	FirstPersonGunMesh->CastShadow = false;
	FirstPersonGunMesh->bCastDynamicShadow = false;
	FirstPersonGunMesh->SetOnlyOwnerSee(true);

	DynamicMuzzleLocComp->SetupAttachment(FirstPersonGunMesh);


}

// Called when the game starts or when spawned
void AMasteringWeaponUE4BK::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMasteringWeaponUE4BK::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMasteringWeaponUE4BK::Fire(FRotator PlayerControlRotation, class UAnimInstance* animationInstance, class UMasteringInventoryUE4BK* Inventory)
{
	if (ProjectileClass != nullptr)
	{
		UWorld* const CurrentWorld = GetWorld();
		if (CurrentWorld != nullptr)
		{
			//Instantiate the projectile---> an instance of the projectile object class will be instantiated! Spawned!
			FVector SpawnLocation = (DynamicMuzzleLocComp != nullptr) ? DynamicMuzzleLocComp->GetComponentLocation() : GetActorLocation() + gunOffset;
			//DISCOVERY:
			//Right here, we are spawning the projectile bullet! 
			//But then, the projectile bullet is spawned based of the scene component called DynamicMuzzleLocComp which we defined earlier, 
			//and of which can be seen in the blueprint of this weap class. 
			//So, just as a precaution should this component ever disappear/not be found, the projectile can still be fired, 
			//but based off the gun offset coordinates which the prior said component posesses, relative to the weapon itself!!! 
			//GetActorLocation() simply gets the root transform location of the gun. It gets world coordinates of the ROOTCOMPONENT of the gun. 
			//ROOTCOMPONENTS Define all the transforms of our class objects, as they should.

			FActorSpawnParameters SomeActorSpawningParameters;
			SomeActorSpawningParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			//DISCOVERY:
			//Kudos to UE4's c++ coding database. FActorSpawnParameters. 
			//A structure defining all the possible Enumerators, each of which represent a particular parameter and possess multiple conditions under this parameter!!!!
			//Therefore, ESpawnActorCollidingHandlingMethod, is simply yet another Enumerator that stores multiple states/conditions. 
			//These states/conditions are tied to various points specific to the represented parameter!!
			//For exp: ESpawnActorCollisionHandlingMethod contains various different enum states - AdjustIfPossibleButAlwaysSpawn (state key = 2), AlwaysSpawn (State key = 1); undefined (State key = 0)
			//Allll these different states make up the conditions for spawning!!!!!!! 
			//EXP: Always spawn means always spawn, adjust if possible... means will adjust but still spawn, 
			//and undefined is as good as not defining an additional spawn parameter, 
			//since the methods of spawning will simply follow the default settings from the actor. (our projectile class)

			CurrentWorld->SpawnActor<AMasteringProjectileUE4BK>(ProjectileClass, SpawnLocation, PlayerControlRotation, SomeActorSpawningParameters);

			if (fireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, fireSound, GetActorLocation(), FireVolumeMultiplier);
			}

			if (fireAnimation != nullptr)
			{
				if (animationInstance != nullptr)
				{
					animationInstance->Montage_Play(fireAnimation, AnimationPlayRate);
				}
			}
		}

	}
	Inventory->ChangeAmmo(GetClass(), -1); //GetClass() simply returns the current class, that defines the fields of this object!!!!
}


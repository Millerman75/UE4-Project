// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MasteringWeaponPickupUE4BK.generated.h"

UCLASS()
class MASTERINGUE4BK_API AMasteringWeaponPickupUE4BK : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMasteringWeaponPickupUE4BK();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY()
	class AMasteringWeaponUE4BK* PickedUpWeapClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Pickup")
		class USkeletalMeshComponent* WeaponMeshRenderer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Pickup")
		class USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Pickup")
		TSubclassOf<class AMasteringWeaponUE4BK> WeaponPickup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Pickup")
		uint8 ammunition = 36;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Pickup")
		int AllocatedAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Pickup")
		int PowerLevel;

	UFUNCTION()
		class AMasteringWeaponUE4BK* GetPickedUpWeaponClassRef() const { return PickedUpWeapClass; };
	

};

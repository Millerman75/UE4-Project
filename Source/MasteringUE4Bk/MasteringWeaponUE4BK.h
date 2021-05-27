// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/UnrealString.h"
#include "MasteringWeaponUE4BK.generated.h"

UCLASS()
class MASTERINGUE4BK_API AMasteringWeaponUE4BK : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMasteringWeaponUE4BK();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		FVector gunOffset = FVector(100.0f, 0.0f, 10.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		class UAnimMontage* fireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		class USoundBase* fireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		TSubclassOf<class AMasteringProjectileUE4BK> ProjectileClass;
	//Right here, we are creating a reference to the class AMasteringUE4BKProjectile, storing it as an instance to a variable called ProjectileClass. 
	//So, from ProjectileClass we can call all things belonging to the AMasteringUE4BKProjectile Class!!!!!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay_Gun")
		class USceneComponent* DynamicMuzzleLocComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay_Gun")
		class USkeletalMeshComponent* FirstPersonGunMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay_Gun")
		float FireVolumeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay_Gun")
		float AnimationPlayRate;

	UFUNCTION()
		void Fire(FRotator PlayerControlRotation, class UAnimInstance* animationInstance, class UMasteringInventoryUE4BK* Inventory);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

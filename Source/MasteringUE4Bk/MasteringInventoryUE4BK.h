// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MasteringWeaponUE4BK.h"
#include "MasteringInventoryUE4BK.generated.h"

USTRUCT()
struct FWeaponProperties
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
		TSubclassOf<class AMasteringWeaponUE4BK> WeaponClass;
	
	UPROPERTY()
		int8 Ammo;

	UPROPERTY()
		int8 WeaponPowerLevel;

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASTERINGUE4BK_API UMasteringInventoryUE4BK : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMasteringInventoryUE4BK();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		TSubclassOf<class AMasteringWeaponUE4BK> DefaultWeapon;

	void SelectNextAvailableWeapon();

	void SelectWeapon(TSubclassOf<class AMasteringWeaponUE4BK> WeaponToEquip);

	void AddNewWeapon(TSubclassOf<class AMasteringWeaponUE4BK> PickedUpWeapon, int AllocatedAmmo, uint8 WeaponPowerLevel);

	void AddDefaultWeapon(); //add any random dfault weapj we have been set with.

	//Get current Selected Weapon
	UFUNCTION()
		FORCEINLINE TSubclassOf<class AMasteringWeaponUE4BK> GetEquippedWeapon() const { return CurrentWeapon; };

	UFUNCTION()
		void ChangeAmmo(TSubclassOf<class AMasteringWeaponUE4BK> EquippedWeapon, const int ChangeAmount);

	UFUNCTION()
		class AMasteringWeaponUE4BK* GetReferencedWeapClass()  const { return WeaponClassRef; };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	TSubclassOf<class AMasteringWeaponUE4BK> CurrentWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<FWeaponProperties> WeaponsArray;
	
	UPROPERTY()
	class AMasteringUE4BkCharacter* WeapOwner;

	UPROPERTY()
	int CurrentWeaponPower = -1;

	UPROPERTY()
	class AMasteringWeaponUE4BK* WeaponClassRef;

};

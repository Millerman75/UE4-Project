// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringInventoryUE4BK.h"
#include "MasteringUE4BkCharacter.h"

// Sets default values for this component's properties
UMasteringInventoryUE4BK::UMasteringInventoryUE4BK()
{
	// ...
	PrimaryComponentTick.bCanEverTick = true;
	WeapOwner = Cast<AMasteringUE4BkCharacter>(GetOwner());

}


// Called when the game starts
void UMasteringInventoryUE4BK::BeginPlay()
{
	Super::BeginPlay();
}

void UMasteringInventoryUE4BK::AddNewWeapon(TSubclassOf<class AMasteringWeaponUE4BK> PickedUpWeapon, int AllocatedAmmo, uint8 PowerLevel)
{
	if (WeaponsArray.Num() > 0)
	{
		for (auto WeaponIterator = WeaponsArray.CreateIterator(); WeaponIterator; ++WeaponIterator)
		{
			FWeaponProperties& CurrentWeapProperties = *WeaponIterator;
			if (CurrentWeapProperties.WeaponClass == PickedUpWeapon)
				//checking if both the ref (alias) and PickedUpWeapon are the same class instance. For example, there are many instances of MasteringWeaponUE4BK in the scene, each with different bp names. So, to check if one of them is in the list, 
				//then we would want to give the weapon ammo (since it is already in the inventory). Else, we would add it in. (Below)
			{
				CurrentWeapProperties.Ammo += AllocatedAmmo;
				return; //Thats why return, cus if we have found the weapon and it exists in our array inventory, no need care readi. Can just return.
			}
		}
		//Else, if the weapon does not exist in the array, then....
		FWeaponProperties WeaponProperties;
		WeaponProperties.WeaponClass = PickedUpWeapon;
		WeaponProperties.WeaponPowerLevel = PowerLevel;
		WeaponProperties.Ammo = AllocatedAmmo;

		WeaponsArray.Add(WeaponProperties); //add to the array
	}
}

void UMasteringInventoryUE4BK::SelectWeapon(TSubclassOf<class AMasteringWeaponUE4BK> WeaponToEquip)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Selecting Weapon"));
	if (WeapOwner != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("there"));
		WeapOwner->EquipWeapon(WeaponToEquip);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Selecting Weapon!!"));
	}

	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("not there"));
	}
	
	

}

void UMasteringInventoryUE4BK::SelectNextAvailableWeapon()
{
	if (WeaponsArray.Num() > 0)
	{
		for (auto weaponIterator = WeaponsArray.CreateIterator(); weaponIterator; weaponIterator++)
		{
			FWeaponProperties &listedWeaponProps = *weaponIterator;
			SelectWeapon(listedWeaponProps.WeaponClass);
			break;
		}
	}


}

//ADD THE DEFAULT WEAPON AT THE START FIRST!!!! Otherwise, no weapon in the array to begin with. Then how to select next available weapon from the above function???!
void UMasteringInventoryUE4BK::AddDefaultWeapon()
{
	FWeaponProperties defaultWeapProps;
	defaultWeapProps.WeaponClass = DefaultWeapon;
	defaultWeapProps.WeaponPowerLevel = 0;
	WeaponsArray.Add(defaultWeapProps);
	SelectWeapon(DefaultWeapon);
}


void UMasteringInventoryUE4BK::ChangeAmmo(TSubclassOf<class AMasteringWeaponUE4BK> EquippedWeapon, const int ChangeAmount)
{
	if (WeaponsArray.Num() > 0)
	{
		for (auto weaponIterator = WeaponsArray.CreateIterator(); weaponIterator; ++weaponIterator)
		{
			//DEBUGMESSAGE("Value %i", weaponIterator);
			FWeaponProperties& currentWeapProps = *weaponIterator;
			//de-referencing the pointer to the instance of the weapon class (Equipped weapon), 
			//and using a reference (&) to create an alias to this weapon class instance variable. 
			//It is from this alias (via memory address) that we can read into the values held within, (BELOW!!!)
			//and not create a variable solely for this (save space and memory).
			if (currentWeapProps.WeaponClass == EquippedWeapon)
			{
				if (currentWeapProps.Ammo == -1) //unlimited ammo, so dun need deduct.
				{
					return;
				}

				else
				{
					currentWeapProps.Ammo = FMath::Clamp(currentWeapProps.Ammo + ChangeAmount, 0, 999);
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, FString::Printf(TEXT("Current Ammo %d"), *FString::FromInt(currentWeapProps.Ammo)));
					}

				}
				if (currentWeapProps.Ammo == 0)
				{
					currentWeapProps.WeaponPowerLevel -= 1;
					SelectNextAvailableWeapon();
					return;
				}


			}
		}
	}
}


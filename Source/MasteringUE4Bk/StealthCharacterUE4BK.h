// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MasteringUE4BkCharacter.h"
#include "StealthCharacterUE4BK.generated.h"

/**
 * 
 */
UCLASS()
class MASTERINGUE4BK_API AStealthCharacterUE4BK : public AMasteringUE4BkCharacter
{
	GENERATED_BODY()

public:
	//Modifier to turn and pitch rate, when in stealth mode!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		float stealthPitchYawScale = 0.5f;

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void AddControllerPitchInput(float mouseInputVal) override;
	void AddControllerYawInput(float mouseInputVal) override;

	virtual void SetStealth();
	virtual void SetUnStealth();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		bool bisStealthed = false;
};

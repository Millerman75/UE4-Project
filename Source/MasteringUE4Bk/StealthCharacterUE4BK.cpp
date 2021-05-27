// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthCharacterUE4BK.h"

void AStealthCharacterUE4BK::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Stealth", IE_Pressed, this, &AStealthCharacterUE4BK::SetStealth);
	PlayerInputComponent->BindAction("Stealth", IE_Released, this, &AStealthCharacterUE4BK::SetUnStealth);

	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AStealthCharacterUE4BK::AddControllerPitchInput(float mouseInputVal)
{
	const float cameraPitchScaleValue = (bisStealthed) ? stealthPitchYawScale : 1.0f;

	Super::AddControllerPitchInput(mouseInputVal * cameraPitchScaleValue);
}

void AStealthCharacterUE4BK::AddControllerYawInput(float mouseInputVal)
{
	const float cameraYawScaleValue = (bisStealthed) ? stealthPitchYawScale : 1.0f;

	Super::AddControllerYawInput(mouseInputVal * cameraYawScaleValue);
}

void AStealthCharacterUE4BK::SetStealth()
{
	bisStealthed = true;
	Super::Crouch();
}

void AStealthCharacterUE4BK::SetUnStealth()
{
	bisStealthed = false;
	Super::UnCrouch();
}



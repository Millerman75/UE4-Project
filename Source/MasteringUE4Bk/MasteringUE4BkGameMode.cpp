// Copyright Epic Games, Inc. All Rights Reserved.

#include "MasteringUE4BkGameMode.h"
#include "MasteringUE4BkHUD.h"
#include "MasteringUE4BkCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMasteringUE4BkGameMode::AMasteringUE4BkGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AMasteringUE4BkHUD::StaticClass();
}

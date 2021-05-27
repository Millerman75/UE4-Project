// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MasteringUE4BkHUD.generated.h"

UCLASS()
class AMasteringUE4BkHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMasteringUE4BkHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};


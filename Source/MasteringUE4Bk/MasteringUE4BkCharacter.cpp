// Copyright Epic Games, Inc. All Rights Reserved.

#include "MasteringUE4BkCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "MasteringWeaponUE4BK.h"
#include "MasteringInventoryUE4BK.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

class UWorld* AMasteringUE4BkCharacter::GetWorld() const
{
	auto OuterMasteringUE4BKChar = GetOuter();
	if (OuterMasteringUE4BKChar)
	{
		return OuterMasteringUE4BKChar->GetWorld();
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("djieidjeidj"));
	}
	return nullptr;
}


//////////////////////////////////////////////////////////////////////////
// AMasteringUE4BkCharacter

AMasteringUE4BkCharacter::AMasteringUE4BkCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;



	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

	Inventory = CreateDefaultSubobject<UMasteringInventoryUE4BK>(TEXT("Player Inventory"));

	bGenerateOverlapEventsDuringLevelStreaming = true;
	
}

void AMasteringUE4BkCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	Inventory->AddDefaultWeapon();

	
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, TEXT("Playing!"));
}

void AMasteringUE4BkCharacter::OnFire()
{
	if (GetEquippedWeapon() != nullptr) 
	//checking if our reference to an instance (EquippedWeapon) of the class AMasteringWeaponUE4BK, exists or not. 
	//Thus, actually the class AMasteringWeaponUE4BK shld be more appropriately called Weapon.
	{
		UAnimInstance* animInstance = Mesh1P->GetAnimInstance();
		GetEquippedWeapon()->Fire(GetControlRotation(), animInstance, Inventory);	
	}
}
//DISCOVERY:
//What is TSubClassOf<>??? 
//Well, it is quite simple. Normally, we define a class as UClass* etc etc. 
//So the same thing can be: UClass* {class name} Weapon. 
//But then, this opens the ENTIRE UE4 Database to the search, for this class should comparisons need to be made. 
//Hence, when we are SURE that what we need is tied to a specific class, then we can simply reference the exact class of AMasteringWeaponUE4BK, 
//as well as all other classes related to it. Thats all! 
//So, there is only a bunch of classes all of which are related to AMasteringWeaponUE4BK, which are up for searching. 
//Therefore, this also enables type safety. What is Type Safety? Well, if one UClass (unreal class) was defined and forced into this function call, 
//the compiler needs to compare it to the class AMasteringWeaponUE4BK. 
//But if this class has no relevance, the variable of this class instance would be a nullptr, leading to errors. 
//TSubClassOf<> limits the classes to exactly the class we want specifically, which is much safer and more efficient.
void AMasteringUE4BkCharacter::EquipWeapon(TSubclassOf<class AMasteringWeaponUE4BK> Weapon) //So, EquipWeapon takes in a class instance of AMasteringWeaponUE4BK.
{
	UWorld* World = GetWorld();

	if (World == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("No World"));
		return;
	}

	if (EquippedWeapon != nullptr)
	{
		EquippedWeapon->Destroy();
	}

	const FRotator SpawnRotation = GetActorRotation();
	const FVector SpawnLocation = GetActorLocation();
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ActorSpawnParams.Owner = this;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Spawning..."));

	EquippedWeapon = Cast<AMasteringWeaponUE4BK>(World->SpawnActor(Weapon, &SpawnLocation, &SpawnRotation, ActorSpawnParams));
	//Attach gun mesh component to skeleton
	EquippedWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint")); 
	//Attach the weapon to the socket in the skeletal SK_MannequinArms mesh, the socket called GripPoint. This can be found when viewing the skeleton of the SK_MannequinArms asset
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMasteringUE4BkCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMasteringUE4BkCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMasteringUE4BkCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMasteringUE4BkCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMasteringUE4BkCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMasteringUE4BkCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMasteringUE4BkCharacter::OnFire);
}

void AMasteringUE4BkCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMasteringUE4BkCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AMasteringUE4BkCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AMasteringUE4BkCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AMasteringUE4BkCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMasteringUE4BkCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMasteringUE4BkCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMasteringUE4BkCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AMasteringUE4BkCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AMasteringUE4BkCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AMasteringUE4BkCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AMasteringUE4BkCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}

// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ThirdRPGCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "TimerManager.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

//////////////////////////////////////////////////////////////////////////
// AThirdRPGCharacter

AThirdRPGCharacter::AThirdRPGCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AThirdRPGCharacter::OnOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AThirdRPGCharacter::OnEndOverlap);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	IsCameraLerping = IsDodge= IsAiming = IsFiring = false;
	CameraLerpTime = FireTimer = TrapTimer = 0.0f;
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}
void AThirdRPGCharacter::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->ActorHasTag(TEXT("GunProjectile")))
		{
			Health -= 10;
			if (Health <= 0)
			{
				//Destroy();
			}
		}
		if (OtherActor->ActorHasTag(TEXT("Explosion")))
		{
			Health -= 30;
			if (Health <= 0)
			{				
				//Destroy();
			}
		}
	}
}
void AThirdRPGCharacter::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
}
void AThirdRPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsCameraLerping)
	{
		CameraLerpTime += GetWorld()->GetDeltaSeconds() * CameraLerpRate;
		LerpCamera();
	}
	FireTimer -= GetWorld()->GetDeltaSeconds();
	TrapTimer -= GetWorld()->GetDeltaSeconds();
	if (IsFiring && FireTimer<=0.0f)
	{
		FireTimer = FireCooldown;
		ActionFire();
	}
}
void AThirdRPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AThirdRPGCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("AimDownSight", IE_Pressed, this, &AThirdRPGCharacter::AimDownSight);
	PlayerInputComponent->BindAction("AimDownSight", IE_Released, this, &AThirdRPGCharacter::StopAim);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AThirdRPGCharacter::ActionDodge);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AThirdRPGCharacter::ToggleIsFiring);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AThirdRPGCharacter::ToggleIsFiring);
	PlayerInputComponent->BindAction("Trap", IE_Pressed, this, &AThirdRPGCharacter::ActionPlaceTrap);




	PlayerInputComponent->BindAxis("MoveForward", this, &AThirdRPGCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AThirdRPGCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AThirdRPGCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AThirdRPGCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AThirdRPGCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AThirdRPGCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AThirdRPGCharacter::OnResetVR);
}


void AThirdRPGCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AThirdRPGCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AThirdRPGCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AThirdRPGCharacter::AimDownSight()
{
	CameraTargetDistance = 180.0f;
	CameraTargetOffset = FVector(0, 150, 70.0f);
	IsCameraLerping = true;
	//UE_LOG(LogTemp, Warning, TEXT("CameraLerping"));
	CameraStartDistance = CameraBoom->TargetArmLength;
	CameraStartOffset = CameraBoom->SocketOffset;
	CameraLerpTime = 0.0f;

	FollowCamera->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	IsAiming = true;
	ToggleCrosshair();


}
void AThirdRPGCharacter::StopAim()
{
	CameraTargetDistance = 340.0f;
	CameraTargetOffset = FVector(0, 0, 30.0f);
	IsCameraLerping = true;
	//UE_LOG(LogTemp, Warning, TEXT("CameraLerping"));
	CameraStartDistance = CameraBoom->TargetArmLength;
	CameraStartOffset = CameraBoom->SocketOffset;
	CameraLerpTime = 0.0f;

	FollowCamera->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	IsAiming = false;
	ToggleCrosshair();

}
void AThirdRPGCharacter::ActionDodge()
{
	if (!GetMovementComponent()->IsFalling())
	{
		IsDodge = true;

		LaunchCharacter(FVector(0, 0, 1) * 250 + GetVelocity(), false, false);
		GetWorldTimerManager().SetTimer(DodgeTimerHandle, this, &AThirdRPGCharacter::ResetActionDodge, DodgeCooldown, false);
	}
}
void AThirdRPGCharacter::ActionFire()
{
	FVector startPos = GetActorLocation() + GetActorForwardVector() * 15 + FVector::UpVector * 20;
	FVector endPos;
	if (!IsAiming)
	{
		endPos = startPos + GetActorForwardVector() * 10000 + FVector::UpVector * 20;
	}
	else
	{
		endPos = FollowCamera->GetComponentLocation() + FollowCamera->GetForwardVector() * 10000 + FollowCamera->GetRightVector() * 600 + FVector::UpVector * 100;
	}
	FHitResult outHit;
	FCollisionQueryParams collisionParams;

	DrawDebugLine(GetWorld(), startPos, endPos, FColor::Green, true);
	if (GetWorld()->LineTraceSingleByChannel(outHit, startPos, endPos, ECC_Visibility, collisionParams))
	{
		if (outHit.bBlockingHit)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Hit %s"),*outHit.GetActor()->GetName());
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			auto dest = outHit.Location;
			auto rotateDirection = dest - startPos;
			rotateDirection = FVector(rotateDirection.X, rotateDirection.Y, rotateDirection.Z);
			FRotator newrot = FRotationMatrix::MakeFromX(rotateDirection).Rotator();
			
			GetWorld()->SpawnActor<AActor>(MyProjectile, startPos, newrot, spawnParams);
		}
	}
}
void AThirdRPGCharacter::ToggleIsFiring()
{
	IsFiring = !IsFiring;
}
void AThirdRPGCharacter::ActionPlaceTrap()
{
	if (TrapTimer > 0)
		return;
	TrapTimer = TrapCooldown;
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	auto dest = GetActorLocation()+ GetActorForwardVector() * 100 + FVector::UpVector * 50;
	GetWorld()->SpawnActor<AActor>(MyTrap, dest, GetActorRotation(), spawnParams);
}
void AThirdRPGCharacter::ResetActionDodge()
{
	IsDodge = false;
}
bool AThirdRPGCharacter::GetDodge()
{
	return IsDodge;
}

void AThirdRPGCharacter::LerpCamera()
{
	
	if (CameraLerpTime >= 1.0f)
	{
		CameraLerpTime = 1.0f;
		IsCameraLerping = false;
		//UE_LOG(LogTemp, Warning, TEXT("Camera Stopped"));
	}

	CameraBoom->TargetArmLength = FMath::Lerp(CameraStartDistance, CameraTargetDistance, CameraLerpTime);
	CameraBoom->SocketOffset = FMath::Lerp(CameraStartOffset, CameraTargetOffset, CameraLerpTime);

}

void AThirdRPGCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AThirdRPGCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AThirdRPGCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AThirdRPGCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

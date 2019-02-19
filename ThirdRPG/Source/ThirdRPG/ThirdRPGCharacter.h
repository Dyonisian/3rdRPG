// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdRPGCharacter.generated.h"

UCLASS(config=Game)
class AThirdRPGCharacter : public ACharacter
{
	GENERATED_BODY()

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
public:
	AThirdRPGCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	//Aiming and shooting
	void AimDownSight();
	void StopAim();
	void LerpCamera();
	void ActionFire();
	bool IsCameraLerping, IsAiming;
	float CameraLerpTime, CameraTargetDistance, CameraStartDistance;
	const float CameraLerpRate = 4.0f;
	FVector CameraTargetOffset, CameraStartOffset;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> MyProjectile;
	UPROPERTY(BlueprintReadOnly)
	bool IsFiring;
	void ToggleIsFiring();
	const float FireCooldown = 0.2f;
	float FireTimer;

	//Place trap
	void ActionPlaceTrap();
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> MyTrap;
	const float TrapCooldown = 3.0f;
	float TrapTimer;
	
	//Dodge
	void ActionDodge();
	void ResetActionDodge();
	FTimerHandle DodgeTimerHandle;
	const float DodgeCooldown = 0.2f;
	bool IsDodge;

	//Health
	const float MaxHealth = 200.0f;


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	bool GetDodge();
	UFUNCTION(BlueprintImplementableEvent)
	void ToggleCrosshair();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyModule.h"
#include "EnemyPawn.generated.h"

UCLASS()
class THIRDRPG_API AEnemyPawn : public APawn
{
	GENERATED_BODY()
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Sets default values for this pawn's properties
	AEnemyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Health
	const float MaxHealth = 400.0f;
	float Health;
	float FlashTimer;
	bool IsFlashing;
	const float FlashCooldown = 0.1f;

	//Modules
	void AddModules();
	int ModuleCount;
	int MaxModules = 24;
	TArray<FVector> ModulePositions;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDestroyEvent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* CollisionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class AEnemyModule>> ModuleList;

	UFUNCTION(BlueprintImplementableEvent)
		void FlashRed();

	UFUNCTION(BlueprintImplementableEvent)
		void ResetMaterial();

	int GetModuleCount() const { return ModuleCount; }
	void IncrementModuleCount() { ModuleCount++; }
	int GetMaxModules() const { return MaxModules; }

};

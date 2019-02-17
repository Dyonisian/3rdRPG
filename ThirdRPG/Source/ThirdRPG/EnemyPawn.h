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
	const float MaxHealth = 200.0f;
	float Health;
	void AddModules();
	int ModuleCount;
	const int MaxModules = 24;
	TArray<FVector> ModulePositions;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* CollisionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<class AEnemyModule>> ModuleList;

	int GetModuleCount() const { return ModuleCount; }
	void IncrementModuleCount() { ModuleCount++; }
	int GetMaxModules() const { return MaxModules; }

};

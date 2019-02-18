// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyModule.generated.h"

UENUM(BlueprintType)
enum class EModuleTypes : uint8
{
	S_Holder UMETA(DisplayName = "Holder"),
	S_Armour UMETA(DisplayName = "Armour"),
	S_Weak UMETA(DisplayName = "Weak"),
	S_Gun UMETA(DisplayName= "Gun"),
	S_Missile UMETA(DisplayName= "Missile")
};

class AEnemyPawn;
UCLASS()
class THIRDRPG_API AEnemyModule : public AActor
{
	GENERATED_BODY()

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:	
	// Sets default values for this actor's properties
	AEnemyModule();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	const float MaxHealth = 200.0f;
	float Health;

	bool IsModuleActive;

	void AddModules();
	void ActionFire();
	void OnZeroHealth();
	void DestroySelf();

	const float GunCooldown = 0.2f;
	const float MissileCooldown = 3.0f;
	const float FlashCooldown = 0.1f;
	bool IsFlashing;
	int GunBurstCount = 0;

	float StateTimer;
	float FlashTimer;
	FTimerHandle DestroyTimerHandle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> GunProjectile;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> MissileProjectile;

	TArray<FVector> ModulePositions;

	AEnemyPawn* OwnerPawn;

	bool IsModuleAdded;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* CollisionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EModuleTypes ModuleType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class AEnemyModule>> ModuleList;

	EModuleTypes GetModuleType() { return ModuleType; }
	void SetModuleType(EModuleTypes moduleType) { ModuleType = moduleType; IsModuleActive = true; }

	void SetOwnerPawn(AEnemyPawn* pawn) { OwnerPawn = pawn; }

	UFUNCTION(BlueprintImplementableEvent)
	void OnDestroyEvent();

	UFUNCTION(BlueprintImplementableEvent)
	void FlashRed();

	UFUNCTION(BlueprintImplementableEvent)
	void ResetMaterial();
};

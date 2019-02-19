// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UENUM(BlueprintType)
enum class EAIStates : uint8
{
	S_Idle UMETA(DisplayName="Idle"),
	S_Patrol UMETA(DisplayName = "Patrol"),
	S_Attack UMETA(DisplayName = "Attack")
};
class AEnemyPawn;
/**
 * 
 */
UCLASS()
class THIRDRPG_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

protected:
	AEnemyPawn* ControlledCharacter;
	APawn* PlayerPawn;

	float StateCooldownTimer;
	const float StateCooldown = 5.0f;

	void Patrol();
	void Attack();

	bool GetPlayerAndPawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = 4000.0f;

public:
	EAIStates AIState;

	
};

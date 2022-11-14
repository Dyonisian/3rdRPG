// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI/GOAP/IGoap.h"
#include "ModuleGOAPController.generated.h"

class AEnemyPawn;

/**
 * 
 */
UCLASS()
class THIRDRPG_API AModuleGOAPController : public AAIController, public IIGoap
{
	GENERATED_BODY()
	
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	

protected:
	AEnemyPawn* ControlledCharacter;
	APawn* PlayerPawn;
	bool GetPlayerAndPawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackRange = 4000.0f;
	void Wander();
	bool HasReachedTarget = false;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Goap")
		TMap<FString, bool> GetWorldState(); virtual TMap<FString, bool> GetWorldState_Implementation() override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Goap")
		TMap<FString, bool> CreateGoalState();virtual TMap<FString, bool> CreateGoalState_Implementation() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Goap")
		bool MoveAgent(UGoapActionC* NextAction); virtual	bool MoveAgent_Implementation(UGoapActionC* NextAction) override;
};

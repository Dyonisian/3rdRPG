// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GOAP/ModuleGOAPController.h"
#include "NavigationSystem.h"
#include "AI/GOAP/GoapActionC.h"
#include "EnemyPawn.h"


void AModuleGOAPController::Tick(float DeltaTime)
{
}

void AModuleGOAPController::BeginPlay()
{
	GetPlayerAndPawn();
}

bool AModuleGOAPController::GetPlayerAndPawn()
{
	bool result = true;
	ControlledCharacter = Cast<AEnemyPawn>(GetPawn());
	if (!ControlledCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI - Controlled pawn not found"));
		result = false;
	}
	PlayerPawn = Cast<APawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player - Controlled pawn not found"));
		result = false;
	}
	return result;
}

void AModuleGOAPController::Wander()
{
	if (!ControlledCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to Patrol, controlled character not found"));
		bool result = GetPlayerAndPawn();
		if (!result)
			return;
	}
	auto dest = UNavigationSystemV1::GetRandomReachablePointInRadius(ControlledCharacter, ControlledCharacter->GetActorLocation(), 10000);
	MoveToLocation(dest);
}

TMap<FString, bool> AModuleGOAPController::GetWorldState_Implementation()
{
	//TODO - HasReachedTarget should be set true when seek's goal is achieved. Then false again when out of range.
	TMap<FString, bool> newMap;
	newMap.Add("HasReachedTarget", HasReachedTarget);
	return newMap;
}

TMap<FString, bool> AModuleGOAPController::CreateGoalState_Implementation()
{
	TMap<FString, bool> newState;
	newState.Add("HasReachedTarget", true);
	return TMap<FString, bool>();
}

bool AModuleGOAPController::MoveAgent_Implementation(UGoapActionC* NextAction)
{
	if (FVector::DistSquared(ControlledCharacter->GetActorLocation(), PlayerPawn->GetActorLocation()) <= AttackRange * AttackRange)
	{
		MoveToActor(NextAction->Target);
	}
	else
	{
		Wander();
	}
	return false;
}

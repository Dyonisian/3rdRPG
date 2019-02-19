// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "NavigationSystem.h"
#include "EnemyPawn.h"
#include "Engine/World.h"
#include "Engine/Classes/GameFramework/PlayerController.h"


void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	StateCooldownTimer -= GetWorld()->GetDeltaSeconds();
	if (StateCooldownTimer <= 0.0f)
	{
		StateCooldownTimer = StateCooldown;
		AIState = static_cast<EAIStates>(FMath::RandRange(0, 3));
		switch (AIState)
		{
		case EAIStates::S_Idle:
			break;
		case EAIStates::S_Patrol:
			Patrol();
			break;
		case EAIStates::S_Attack:
			Attack();
			break;
		}
		
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	AIState = EAIStates::S_Idle;
	StateCooldownTimer = StateCooldown;
	GetPlayerAndPawn();
}

void AEnemyAIController::Patrol()
{

	FNavLocation loc;
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

void AEnemyAIController::Attack()
{
	if (!ControlledCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to attack, controlled character not found"));
		bool result = GetPlayerAndPawn();
		if (!result)
			return;
	}	
	if (FVector::DistSquared(ControlledCharacter->GetActorLocation(), PlayerPawn->GetActorLocation()) <= AttackRange * AttackRange)
	{
		MoveToActor(PlayerPawn);
	}
	else
	{
		Patrol();
	}
}

bool AEnemyAIController::GetPlayerAndPawn()
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

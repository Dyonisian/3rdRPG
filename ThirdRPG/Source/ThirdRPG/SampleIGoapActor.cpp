// Fill out your copyright notice in the Description page of Project Settings.

#include "SampleIGoapActor.h"

// Sets default values
ASampleIGoapActor::ASampleIGoapActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



// Called when the game starts or when spawned
void ASampleIGoapActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASampleIGoapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TMap<FString, bool> ASampleIGoapActor::GetWorldState_Implementation()
{
	return TMap<FString, bool>();
}

TMap<FString, bool> ASampleIGoapActor::CreateGoalState_Implementation()
{
	return TMap<FString, bool>();
}

void ASampleIGoapActor::PlanFailed_Implementation(const TMap<FString, bool>& FailedGoal)
{
}



void ASampleIGoapActor::PlanFound_Implementation(const TMap<FString, bool>& Goal, const TArray<UGoapActionC*>& Actions)
{
}

void ASampleIGoapActor::ActionsFinished_Implementation()
{
}

void ASampleIGoapActor::PlanAborted_Implementation(UGoapActionC * Aborter)
{
}
bool ASampleIGoapActor::MoveAgent_Implementation(UGoapActionC * NextAction)
{
	return false;
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "GoapActionC.h"

// Sets default values for this component's properties
UGoapActionC::UGoapActionC()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UGoapActionC::Reset()
{
}

bool UGoapActionC::IsDone()
{
	return false;
}

bool UGoapActionC::CheckProceduralPrecondition(AActor * actor)
{
	return false;
}

bool UGoapActionC::Perform(AActor * actor)
{
	return false;
}

bool UGoapActionC::RequiresInRange()
{
	return false;
}

TMap<FString, bool> UGoapActionC::GetPreconditions()
{
	return Preconditions;
}

// Called when the game starts
void UGoapActionC::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGoapActionC::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


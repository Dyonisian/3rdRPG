// Fill out your copyright notice in the Description page of Project Settings.

#include "GoapAgent.h"
#include "../FSM/FSM2.h"

// Sets default values for this component's properties
UGoapAgent::UGoapAgent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	if (!StateMachine)
	{
		StateMachine = CreateDefaultSubobject<UFSM2>(TEXT("FiniteStateMachine"));
	}
	// ...
}


// Called when the game starts
void UGoapAgent::BeginPlay()
{
	//StateMachine = new UFSM2();
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGoapAgent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


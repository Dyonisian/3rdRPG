// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "../FSM/FSM2.h"
#include "GoapAgent.generated.h"

class AActor;
class UFSM2;
class GoapAction;
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSMSTATE, FSM*, Fsm, AActor*, Actor);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDRPG_API UGoapAgent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoapAgent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFSM2* StateMachine;

	TMap<FString, GoapAction*> AvailableActions;
	TArray<GoapAction*> CurrentActionsQueue;

	//IGoap DataProvider;

	//GoapPlanner Planner;
	 

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

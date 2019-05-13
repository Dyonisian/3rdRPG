// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "../FSM/FSM2.h"
#include "GoapAgent.generated.h"

class AActor;
class UFSM2;
class UGoapActionC;
class IIGoap;
class GoapPlanner;
class FSMSTATE;
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
	//Finds something to do
	FSMSTATE* IdleState; 
	//Moves to a target
	FSMSTATE* MoveToState;
	//Performs an action
	FSMSTATE* PerformActionState;

	TSet<UGoapActionC*> AvailableActions;
	TArray<UGoapActionC*> CurrentActionsQueue;

	//This is the implementing class that provides our world data and listens to feedback on planning
	IIGoap* DataProvider;

	GoapPlanner* Planner;

	void FindDataProvider();
	UFUNCTION()
	//Bound to delegate for idle state
	void CreateIdleState(UFSM2* Fsm, AActor* Actor);
	UFUNCTION()
	//Bound to delegate for move state
	void CreateMoveToState(UFSM2* Fsm, AActor* Actor);
	UFUNCTION()
	//Bound to delegate for perform action state
	void CreatePerformActionState(UFSM2* Fsm, AActor* Actor);
	void LoadActions();
	 

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void AddAction(UGoapActionC* action) { AvailableActions.Add(action); }
	UGoapActionC* GetAction(UGoapActionC* action);
	void RemoveAction(UGoapActionC* action) { AvailableActions.Remove(action); }
	bool HasActionPlan() { return CurrentActionsQueue.Num() > 0; }

};

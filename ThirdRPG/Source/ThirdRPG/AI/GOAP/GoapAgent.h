// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "../FSM/FSM2.h"
#include "GoapAgent.generated.h"

class AActor;
class UFSM2;
class GoapAction;
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
	FSMSTATE* IdleState;
	FSMSTATE* MoveToState;
	FSMSTATE* PerformActionState;

	TSet<GoapAction*> AvailableActions;
	TArray<GoapAction*> CurrentActionsQueue;

	IIGoap* DataProvider;

	GoapPlanner* Planner;

	void FindDataProvider();
	UFUNCTION()
	void CreateIdleState(UFSM2* Fsm, AActor* Actor);
	UFUNCTION()
	void CreateMoveToState(UFSM2* Fsm, AActor* Actor);
	UFUNCTION()
	void CreatePerformActionState(UFSM2* Fsm, AActor* Actor);
	void LoadActions();
	 

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void AddAction(GoapAction* action) { AvailableActions.Add(action); }
	GoapAction* GetAction(GoapAction* action);
	void RemoveAction(GoapAction* action) { AvailableActions.Remove(action); }
	bool HasActionPlan() { return CurrentActionsQueue.Num() > 0; }

};

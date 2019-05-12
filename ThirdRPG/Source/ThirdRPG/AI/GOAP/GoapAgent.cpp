// Fill out your copyright notice in the Description page of Project Settings.

#include "GoapAgent.h"
#include "../FSM/FSM2.h"
#include "GoapPlanner.h"
#include "GoapActionC.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "IGoap.h"

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
	Super::BeginPlay();
	Planner = new GoapPlanner();
	FindDataProvider();
	IdleState->AddDynamic(this, &UGoapAgent::CreateIdleState);
	MoveToState->AddDynamic(this, &UGoapAgent::CreateMoveToState);
	PerformActionState->AddDynamic(this,&UGoapAgent::CreatePerformActionState);
	StateMachine->PushState(IdleState);
	LoadActions();
	// ...
	
}


void UGoapAgent::FindDataProvider()
{
	auto comps = GetOwner()->GetComponents();
	for (auto c : comps)
	{
		IIGoap* inter = Cast<IIGoap>(c);
		if (inter)
		{
			DataProvider = inter;
			break;
		}
	}
}

void UGoapAgent::CreateIdleState(UFSM2* Fsm, AActor* Actor)
{
	//Goap planning
	
	TMap<FString, bool> worldState = IIGoap::Execute_GetWorldState(DataProvider->_getUObject());
	TMap<FString, bool> goal = DataProvider->CreateGoalState();

	TArray<UGoapActionC*> plan = Planner->Plan(GetOwner(), AvailableActions, worldState, goal);
	if (plan.Num()!=0)
	{
		CurrentActionsQueue = plan;
		//DataProvider->PlanFound(goal, plan);
		Fsm->PopState();
		Fsm->PushState(PerformActionState);
	}
	else
	{		
		UE_LOG(LogTemp, Warning, TEXT("Failed plan!"));
		DataProvider->PlanFailed(goal);
		Fsm->PopState();
		Fsm->PushState(IdleState);
	}

}

void UGoapAgent::CreateMoveToState(UFSM2* Fsm, AActor* Actor)
{
	UGoapActionC* action = CurrentActionsQueue[0];
	if (action->RequiresInRange() && action->Target == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Action requires target but has none"));
		Fsm->PopState();
		Fsm->PopState();
		Fsm->PushState(IdleState);
		return;
	}

	//if (DataProvider->MoveAgent(action))
	{
		Fsm->PopState();
	}
}

void UGoapAgent::CreatePerformActionState(UFSM2* Fsm, AActor* Actor)
{
	if (!HasActionPlan())
	{
		UE_LOG(LogTemp, Warning, TEXT("Done actions"));
		Fsm->PopState();
		Fsm->PushState(IdleState);
		DataProvider->ActionsFinished();
		return;
	}
	UGoapActionC* action = CurrentActionsQueue[0];
	if (action->IsDone())
	{
		CurrentActionsQueue.Remove(CurrentActionsQueue[0]);
	}
	if (HasActionPlan())
	{
		action = CurrentActionsQueue[0];
		bool inRange = action->RequiresInRange() ? action->IsInRange() : true;
		if (inRange)
		{
			bool success = action->Perform(Actor);
			if (!success)
			{
				Fsm->PopState();
				Fsm->PushState(IdleState);
			//	DataProvider->PlanAborted(action);
			}
			
		}
		else
		{
			Fsm->PushState(MoveToState);		
		}
	}
	else
	{
		Fsm->PopState();
		Fsm->PushState(IdleState);
		DataProvider->ActionsFinished();
	}
}

void UGoapAgent::LoadActions()
{
	auto actions = GetOwner()->GetComponentsByClass(UGoapActionC::StaticClass());
	for (auto a : actions)
	{
		AvailableActions.Add(Cast<UGoapActionC>(a));
	}
	UE_LOG(LogTemp, Warning, TEXT("Found actions"));
}

// Called every frame
void UGoapAgent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	StateMachine->Update(GetOwner());
	// ...
}

UGoapActionC * UGoapAgent::GetAction(UGoapActionC * action)
{
	for (auto &g : AvailableActions)
	{
		if (g == action)
		{
			return g;
		}
	}
	return nullptr;
}


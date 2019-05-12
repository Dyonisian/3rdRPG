 // Fill out your copyright notice in the Description page of Project Settings.

#include "GoapPlanner.h"
#include "GoapActionC.h"

/*
Sample plan for shooter

Actions
Attack – Cost: Low Preconditions: “In range”, has ammo Effect: Attacking
Seek – Cost: Preconditions: “In sight” Effect: Seeking
Wander – Cost: Very High Preconditions: None Effect: Wandering
Super – Cost: Very Low Preconditions: “Charged” Effect: Super – Double rotation and firing speed
Shield – Cost: High (Lowered when being healed) Preconditions: None Effect: Projects shield
Heal self – Cost: Very High (Reduced by being damaged) Preconditions: "Damaged"  Effect: Healing
Heal ally – Cost: Low (Increased by being damaged) Preconditions: "Damaged ally in range" Effect: Healing ally
Seek Ally – Cost: Low (Increased by being damaged) Preconditions: None Effect: Move towards (damaged) ally
Collect ammo – Cost: High Preconditions: "Low ammo" Effect: Seek charging station

*/
GoapPlanner::GoapPlanner()
{
}

GoapPlanner::~GoapPlanner()
{
}

TArray<UGoapActionC*> GoapPlanner::Plan(AActor * Agent, TSet<UGoapActionC*> AvailableActions, TMap<FString, bool> WorldState, TMap<FString, bool> Goal)
{
	for (auto &a : AvailableActions)
	{
		a->DoReset();
	}

	TSet<UGoapActionC*> UsableActions;
	for (auto &a : AvailableActions)
	{
		if (a->CheckProceduralPrecondition(Agent))
			UsableActions.Add(a);
	}

	TArray<Node*> Leaves;
	//Build graph

	Node* Start = new Node(nullptr,0,WorldState,nullptr);
	bool Success = BuildGraph(Start, Leaves, UsableActions, Goal);

	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("No plan"));
		return TArray<UGoapActionC*>();
	}

	Node* Cheapest = nullptr;
	for (auto n : Leaves)
	{
		if (Cheapest == nullptr)
		{
			Cheapest = n;
		}
		else
		{
			if (n->RunningCost < Cheapest->RunningCost)
			{
				Cheapest = n;
			}
		}
	}

	TArray<UGoapActionC*> Result;
	Node* n = Cheapest;

	while (n != nullptr)
	{
		if (n->Action != nullptr)
		{
			Result.Insert(n->Action, 0);
		}
		n = n->Parent;
	}

	return Result;
}

bool GoapPlanner::BuildGraph(Node * parent, TArray<Node*> Leaves, TSet<UGoapActionC*> UsableActions, TMap<FString, bool> Goal)
{
	bool FoundOne = false;
	for (auto &action : UsableActions)
	{
		if (InState(action->GetPreconditions(), parent->State))
		{
			TMap<FString, bool> CurrentState = PopulateState(parent->State, action->GetEffects());
			Node* node = new Node(parent, parent->RunningCost + action->Cost, CurrentState, action);

			if (InState(Goal, CurrentState))
			{
				Leaves.Add(node);
				FoundOne = true;
			}
			else
			{
				TSet<UGoapActionC*> subset = ActionSubset(UsableActions, action);
				bool found = BuildGraph(node, Leaves, subset, Goal);
				if (found)
					FoundOne = true;
			}
		}
	}
	return FoundOne;
}

TSet<UGoapActionC*> GoapPlanner::ActionSubset(TSet<UGoapActionC*> Actions, UGoapActionC * RemoveMe)
{
	TSet<UGoapActionC*> subset;
	for (auto a : Actions)
	{
		if (a != RemoveMe)
		{
			subset.Add(a);
		}
	}
	return subset;
}

bool GoapPlanner::InState(TMap<FString, bool> Test, TMap<FString, bool> State)
{
	bool allMatch = true;
	for (auto t : Test)
	{
		bool match = false;
		for (auto s : State)
		{
			if (s == t)
			{
				match = true;
				break;
			}
		}
		if (!match)
			allMatch = false;
	}
	return allMatch;
}

TMap<FString, bool> GoapPlanner::PopulateState(TMap<FString, bool> CurrentState, TMap<FString, bool> StateChange)
{
	TMap<FString, bool> state;
	for (auto s : CurrentState)
	{
		state.Add(s.Key, s.Value);
	}
	for (auto change : StateChange)
	{
		bool exists = false;
		FString key;
		for (auto s : state)
		{
			if (s == change)
			{
				exists = true;
				key = s.Key;
				break;
			}
		}
		if (exists)
		{
			state.Remove(key);
			state.Add(change.Key, change.Value);
		}

		else
		{
			state.Add(change.Key, change.Value);
		}
	}
	return state;
}

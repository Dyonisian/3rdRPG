// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
class GoapAction;
class AActor;
/**
  * Plans what actions can be completed in order to fulfill a goal state.
 */
class THIRDRPG_API GoapPlanner
{
public:
	GoapPlanner();
	~GoapPlanner();
	/**
	* Used for building up the graph and holding the running costs of actions.
	*/
	class Node {
	public:
		Node* Parent;
		float RunningCost;
		TMap<FString, bool> State;
		GoapAction* Action;

		Node(Node* parent, float runningCost, TMap<FString, bool> state, GoapAction* action)
		{
			Parent = parent;
			RunningCost = runningCost;
			State = state;
			Action = action;
		}
	};
	/**
	* Plan what sequence of actions can fulfill the goal.
	* Returns null if a plan could not be found, or a list of the actions
	* that must be performed, in order, to fulfill the goal.
	*/
	TArray<GoapAction*> Plan(AActor* Agent, TSet<GoapAction*> AvailableActions, TMap<FString, bool> WorldState, TMap<FString, bool> Goal);

	/**
	* Returns true if at least one solution was found.
	* The possible paths are stored in the leaves list. Each leaf has a
	* 'runningCost' value where the lowest cost will be the best action
	* sequence.
	*/	
	bool BuildGraph(Node* parent, TArray<Node*> Leaves, TSet<GoapAction*> UsableActions, TMap<FString,bool> Goal);
	/**
	* Create a subset of the actions excluding the removeMe one. Creates a new set.
	*/
	TSet<GoapAction*> ActionSubset(TSet<GoapAction*> Actions, GoapAction* RemoveMe);
	/**
	* Check that all items in 'test' are in 'state'. If just one does not match or is not there
	* then this returns false.
	*/
	bool InState(TMap<FString, bool> Test, TMap<FString, bool> State);
	/**
	* Apply the stateChange to the currentState
	*/
	TMap<FString, bool> PopulateState(TMap<FString, bool> CurrentState, TMap<FString, bool> StateChange);


};

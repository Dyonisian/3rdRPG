// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IGoap.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIGoap : public UInterface
{
	GENERATED_BODY()
};
class UGoapActionC;
/**
 * Any agent that wants to use GOAP must implement
 * this interface. It provides information to the GOAP
 * planner so it can plan what actions to use.
 * 
 * It also provides an interface for the planner to give 
 * feedback to the Agent and report success/failure.
 */
class THIRDRPG_API IIGoap
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	* The starting state of the Agent and the world.
	* Supply what states are needed for actions to run.
	*/
	//UFUNCTION(BlueprintCallable, Category = "Goap")
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Goap")
	TMap<FString, bool> GetWorldState();
	/**
	* Give the planner a new goal so it can figure out
	* the actions needed to fulfill it.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Goap")
	TMap<FString, bool> CreateGoalState();
	/**
	* No sequence of actions could be found for the supplied goal.
	* You will need to try another goal
	*/
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Goap")
	void PlanFailed (TMap<FString, bool> FailedGoal);
	/**
	* A plan was found for the supplied goal.
	* These are the actions the Agent will perform, in order.
	*/
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Goap")
	void PlanFound(TMap<FString, bool> Goal, TArray<UGoapActionC*> Actions);
	/**
	* All actions are complete and the goal was reached. Hooray!
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Goap")
	void ActionsFinished();
	/**
	* One of the actions caused the plan to abort.
	* That action is returned.
	*/
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Goap")
	void PlanAborted(UGoapActionC* Aborter);
	/**
	* Called during Update. Move the agent towards the target in order
	* for the next action to be able to perform.
	* Return true if the Agent is at the target and the next action can perform.
	* False if it is not there yet.
	*/
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Goap")
	bool MoveAgent(UGoapActionC* NextAction);

};

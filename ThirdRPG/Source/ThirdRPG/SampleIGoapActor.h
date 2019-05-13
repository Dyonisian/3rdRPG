// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AI/GOAP/IGoap.h"
#include "SampleIGoapActor.generated.h"

UCLASS()
class THIRDRPG_API ASampleIGoapActor : public AActor, public IIGoap
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASampleIGoapActor();
	
	virtual TMap<FString, bool> GetWorldState_Implementation() override;
	/**
	* Give the planner a new goal so it can figure out
	* the actions needed to fulfill it.
	*/
	
	virtual TMap<FString, bool> CreateGoalState_Implementation() override;
	/**
	* No sequence of actions could be found for the supplied goal.
	* You will need to try another goal
	*/
	
	virtual void PlanFailed_Implementation(const TMap<FString, bool>& FailedGoal) override;

	/**
	* A plan was found for the supplied goal.
	* These are the actions the Agent will perform, in order.
	*/
	
	virtual void PlanFound_Implementation(const TMap<FString, bool>& Goal, const TArray<UGoapActionC*>& Actions) override;

	/**
	* All actions are complete and the goal was reached. Hooray!
	*/
	
	virtual void ActionsFinished_Implementation() override;

	/**
	* One of the actions caused the plan to abort.
	* That action is returned.
	*/
	
	virtual void PlanAborted_Implementation(UGoapActionC* Aborter) override;

	/**
	* Called during Update. Move the agent towards the target in order
	* for the next action to be able to perform.
	* Return true if the Agent is at the target and the next action can perform.
	* False if it is not there yet.
	*/
	
	virtual bool MoveAgent_Implementation(UGoapActionC* NextAction) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

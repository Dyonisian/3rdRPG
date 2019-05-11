// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/Delegates/Delegate.h"
class AActor;

/**
 * 
 */
class THIRDRPG_API FSM
{
public:
	FSM();
	~FSM();
	void Update(AActor* Actor);
	//TODO there needs to be a stack of states here
	//FSMSTATE FSMState;
	//void FSMState(FSM fsm, AActor* Actor);
	//void PushState(FSMState* State) { StateStack.Push(State); }
	//void PopState() { StateStack.Pop(); }	
	//
	//FSMSTATE IdleState;
	//FSMSTATE MoveToState;
	//FSMSTATE PerformActionState;

private:
	//TArray<FSMState*> StateStack;
	
};



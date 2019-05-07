// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSMState, FSM, Fsm, AActor*, Actor);
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
	//void FSMState(FSM fsm, AActor* Actor);
	//void PushState(FSMState* State) { StateStack.Push(State); }
	//void PopState() { StateStack.Pop(); }

private:
	//TArray<FSMState*> StateStack;

	FSMState FsmState;

};

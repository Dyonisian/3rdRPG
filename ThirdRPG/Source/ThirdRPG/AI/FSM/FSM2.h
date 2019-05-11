// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSM2.generated.h"

class AActor;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSMSTATE, UFSM2*, Fsm, AActor*, Actor);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDRPG_API UFSM2 : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSM2();
	void Update(AActor* Actor);
	//TODO there needs to be a stack of states here
	
	void FSMState(UFSM2* fsm, AActor* Actor);
	void PushState(FSMSTATE* State) { StateStack.Push(State); }
	void PopState() { StateStack.Pop(); }	
	//
	//FSMSTATE IdleState;
	//FSMSTATE MoveToState;
	//FSMSTATE PerformActionState;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	TArray<FSMSTATE*> StateStack;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

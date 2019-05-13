// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoapActionC.generated.h"

/*
* UGoapActionC is an abstract class. It is meant to be used by creating derived classes. 
* 
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDRPG_API UGoapActionC : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoapActionC();
	/* The cost of performing the action.
	* Figure out a weight that suits the action.
	* Changing it will affect what actions are chosen during planning.*/
	float Cost = 1.0f;
	
	AActor* Target;

	void DoReset() { InRange = false; Target = nullptr; Reset(); };

	virtual void Reset();
	virtual bool IsDone();
	/*Procedurally check if this action can be run*/
	virtual bool CheckProceduralPrecondition(AActor* actor);
	virtual bool Perform(AActor* actor);
	/*Does this action need to be within range of a target actor? Decides if MoveTo has to be run.*/
	virtual bool RequiresInRange();

	bool IsInRange() { return InRange; }
	void SetInRange(bool inRange) { InRange = InRange; }
	void AddPrecondition(FString key, bool condition) { Preconditions.Add(key, condition); }
	void RemovePrecondition(FString key) { Preconditions.Remove(key); }
	void AddEffect(FString key, bool condition) { Effects.Add(key, condition); }
	void RemoveEffect(FString key) { Effects.Remove(key); }

	TMap<FString, bool> GetPreconditions();
	TMap<FString, bool> GetEffects() { return Effects; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	TMap<FString, bool> Preconditions;
	TMap<FString, bool> Effects;

	bool InRange = false;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

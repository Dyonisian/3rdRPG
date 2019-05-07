// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AActor;
/**
 * 
 */
class THIRDRPG_API GoapAction
{
public:
	GoapAction();
	~GoapAction();

	float Cost = 1.0f;
	AActor* Target;

	void DoReset() { InRange = false; Target = nullptr; Reset(); };

	virtual void Reset() = 0;
	virtual bool IsDone() = 0;
	virtual bool checkProceduralPrecondition(AActor* actor) = 0;
	virtual bool Perform(AActor* actor) = 0;
	virtual bool RequiresInRange() = 0;

	bool IsInRange() { return InRange; }
	void SetInRange(bool inRange) { InRange = InRange; }
	void AddPrecondition(FString key, bool condition) { Preconditions.Add(key, condition); }
	void RemovePrecondition(FString key) { Preconditions.Remove(key); }	
	void AddEffect(FString key, bool condition) { Effects.Add(key, condition); }
	void RemoveEffect(FString key) { Effects.Remove(key); }

	TMap<FString, bool>* GetPreconditions() { return &Preconditions; }
	TMap<FString, bool>* GetEffects() { return &Effects; }







private:
	TMap<FString, bool> Preconditions;
	TMap<FString, bool> Effects;

	bool InRange = false;

	



};

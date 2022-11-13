// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GOAP/SeekAction.h"

void USeekAction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
}

void USeekAction::BeginPlay()
{
	AddPrecondition("hasReachedTarget", false);
	AddEffect("hasReachedTarget", true);
}

bool USeekAction::RequiresInRange()
{
	return true;
}

bool USeekAction::CheckProceduralPrecondition(AActor* actor)
{
	return true;
}

bool USeekAction::Perform(AActor* actor)
{
	HasReachedTarget = true;
	return true;
}

void USeekAction::Reset()
{
	HasReachedTarget = false;
}

bool USeekAction::IsDone()
{
	return HasReachedTarget;
}

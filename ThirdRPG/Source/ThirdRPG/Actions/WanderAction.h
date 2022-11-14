// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/GOAP/GoapActionC.h"
#include "WanderAction.generated.h"

/**
 * 
 */
UCLASS()
class THIRDRPG_API UWanderAction : public UGoapActionC
{
	GENERATED_BODY()
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);
	virtual void BeginPlay() override;

public:

	
};

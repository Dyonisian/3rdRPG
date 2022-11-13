// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/GOAP/GoapActionC.h"
#include "SeekAction.generated.h"

/**
 * 
 */
UCLASS()
class THIRDRPG_API USeekAction : public UGoapActionC
{
	GENERATED_BODY()
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);
	virtual void BeginPlay() override;

	bool HasReachedTarget;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackRange = 4000.0f;
public:
	virtual bool RequiresInRange() override;
	virtual bool CheckProceduralPrecondition(AActor* actor) override;
	virtual bool Perform(AActor* actor) override;
	virtual void Reset() override;
	virtual bool IsDone() override;



};

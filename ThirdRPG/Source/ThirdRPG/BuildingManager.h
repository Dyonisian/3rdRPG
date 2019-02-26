// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingManager.generated.h"

class ABuildingBase;
enum class EBuildingTypes : uint8;
class AThirdRPGCharacter;
class ABuildingBase;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDRPG_API UBuildingManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildingManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	EBuildingTypes BuildType;

	

	void PreviewFloor();
	void PreviewWall();
	void PreviewRamp();

	FVector ToGridLocation(FVector loc);
	float ToGridRotation(float rot);
	FVector GetPlayerLookAtLocation(float offset);

	float XGridSize, YGridSize, ZGridSize;
	float RotSnap;

	AThirdRPGCharacter* PlayerCharacter;

	ABuildingBase* CurrentBuildPreview;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void PreviewBuildType(EBuildingTypes buildType);
	void Build();
	void CancelBuildMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class ABuildingBase>> BuildPreviewList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class ABuildingBase>> BuildList;


		
};

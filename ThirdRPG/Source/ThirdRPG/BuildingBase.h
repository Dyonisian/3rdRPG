// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingBase.generated.h"

UENUM(BlueprintType)
enum class EBuildingTypes : uint8
{
	S_Wall UMETA(DisplayName = "Wall"),
	S_Floor UMETA(DisplayName = "Floor"),
	S_Ramp UMETA(DisplayName = "Ramp")
};

UCLASS()
class THIRDRPG_API ABuildingBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	EBuildingTypes BuildType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

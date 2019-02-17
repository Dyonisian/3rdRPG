// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyRoot.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"

// Sets default values
AEnemyRoot::AEnemyRoot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	


}

// Called when the game starts or when spawned
void AEnemyRoot::BeginPlay()
{
	Super::BeginPlay();
	
	
	
}

// Called every frame
void AEnemyRoot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




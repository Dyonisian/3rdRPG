// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyPawn.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"


// Sets default values
AEnemyPawn::AEnemyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent = FindComponentByClass<UStaticMeshComponent>();
	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyPawn::OnOverlap);
		CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AEnemyPawn::OnEndOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Collision and overlap setup failed - Pawn"));
	}
	Health = MaxHealth;
	
}

// Called every frame
void AEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void AEnemyPawn::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->ActorHasTag(TEXT("CharProjectile")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Ouch"));
			Health -= 10;
			if (Health <= 0)
			{
				Destroy();
			}
			OtherActor->Destroy();
		}
	}
}
void AEnemyPawn::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}



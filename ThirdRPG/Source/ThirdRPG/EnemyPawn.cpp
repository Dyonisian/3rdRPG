// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyPawn.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "EnemyModule.h"



// Sets default values
AEnemyPawn::AEnemyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ModulePositions.Push(FVector(0, 0, -1));
	ModulePositions.Push(FVector(-1, 0, 0));
	ModulePositions.Push(FVector(1, 0, 0));
	ModulePositions.Push(FVector(0, -1, 0));
	ModulePositions.Push(FVector(0, 1, 0));
	ModulePositions.Push(FVector(0, 0, 1));

}

// Called when the game starts or when spawned
void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	auto components = GetComponents();
	for (auto &com : components)
	{
		if (com->GetName() == "EnemyPawnCol")
		{
			CollisionComponent = Cast<UStaticMeshComponent>(com);
			break;
		}
	}
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
	AddModules();
	
}

void AEnemyPawn::AddModules()
{
	//Check parent max modules, if exceeded, return
	//Check list
	//loop
	for (int i=0; i<ModulePositions.Num(); i++)
	{
		if (ModuleCount >= MaxModules)
			break;
		//skip bottom position
		auto pos = ModulePositions[i];
		if (pos == FVector(0, 0, -1))
			continue;

		FVector startPos = GetActorLocation() + pos * 170;
		FVector endPos;
		
			endPos = GetActorLocation() + pos * 250;
		
		FHitResult outHit;
		FCollisionQueryParams collisionParams;

		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, true);

		if (GetWorld()->LineTraceSingleByChannel(outHit, startPos, endPos, ECC_Visibility, collisionParams))
		{
			if (outHit.bBlockingHit)
			{
				UE_LOG(LogTemp, Warning, TEXT("Nope, hit something!"));

				continue;
			}
			
		}
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		auto Dest = endPos;
		auto RotateDirection = Dest - startPos;
		RotateDirection = FVector(RotateDirection.X, RotateDirection.Y, RotateDirection.Z);
		FRotator newrot = FRotationMatrix::MakeFromX(RotateDirection).Rotator();
		auto moduleNo = FMath::RandRange(0, 7);
		if (moduleNo > 4)
			moduleNo = 0;
		auto module = ModuleList[moduleNo];

		FVector spawnPos = startPos;
		if (moduleNo == 0)
			spawnPos = endPos;
	
		auto spawnedModule = Cast<AEnemyModule>(GetWorld()->SpawnActor<AActor>(module, spawnPos, newrot, spawnParams));
		FAttachmentTransformRules attachRules(EAttachmentRule::KeepWorld,false);
		spawnedModule->AttachToActor(this, attachRules);
		spawnedModule->SetOwnerPawn(this);
		ModuleCount++;

	}
	//raycast in direction, if blocked, continue
	//else 
	//randomize a part, move into position, add to parent?
	//add to parent's counter
	//if exceeded size, break
}

// Called every frame
void AEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugLine(GetWorld(), GetActorLocation() + ModulePositions[1] * 170 , GetActorLocation() + ModulePositions[1] * 250, FColor::Blue, true);
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



// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyModule.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "EnemyPawn.h"

// Sets default values
AEnemyModule::AEnemyModule()
{
	//ModuleType = EModuleTypes::S_Gun;
	PrimaryActorTick.bCanEverTick = true;
	IsModuleActive = true;
	StateTimer = FMath::RandRange(0.0f, 10.0f);
	ModulePositions.Push(FVector(0, 0, -1));
	ModulePositions.Push(FVector(-1, 0, 0));
	ModulePositions.Push(FVector(1, 0, 0));
	ModulePositions.Push(FVector(0, -1, 0));
	ModulePositions.Push(FVector(0, 1, 0));
	ModulePositions.Push(FVector(0, 0, 1));

	IsModuleAdded = false;

}

// Called when the game starts or when spawned
void AEnemyModule::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent = FindComponentByClass<UStaticMeshComponent>();
	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyModule::OnOverlap);
		CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AEnemyModule::OnEndOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Collision and overlap setup failed - Module"));
	}
	Health = MaxHealth;
	

}
// Called every frame
void AEnemyModule::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsModuleActive)
		return;
	DrawDebugLine(GetWorld(), GetActorLocation() + ModulePositions[1] * 170, GetActorLocation() + ModulePositions[1] * 250, FColor::Blue, true);

	if (ModuleType == EModuleTypes::S_Holder && OwnerPawn && !IsModuleAdded)
	{
		AddModules();
		IsModuleAdded = true;
	}
	StateTimer -= GetWorld()->GetDeltaSeconds();
	if (StateTimer <= 0.0f)
	{
		switch (ModuleType)
		{
		case EModuleTypes::S_Missile: StateTimer = MissileCooldown;
			ActionFire();
			break;
		case EModuleTypes::S_Gun: StateTimer = GunCooldown;
			GunBurstCount++;
			if (GunBurstCount >= 3)
				StateTimer = GunCooldown * 3;
			ActionFire();
			break;
		}
	}
}

void AEnemyModule::AddModules()
{
	///Check parent max modules, if exceeded, return
	//Check list
	//loop
	for (int i = 0; i<ModulePositions.Num(); i++)
	{
		if (OwnerPawn->GetModuleCount() >= OwnerPawn->GetMaxModules())
			break;
		auto pos = ModulePositions[i];
				
		FVector startPos = GetActorLocation() + pos * 170;
		FVector endPos;

		endPos = GetActorLocation() + pos * 250;

		FHitResult outHit;
		FCollisionQueryParams collisionParams;

		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, true);

		if (GetWorld()->LineTraceSingleByChannel(outHit, startPos, endPos, ECC_Visibility, collisionParams))
		{			
				UE_LOG(LogTemp, Warning, TEXT("Nope, hit something!"));
				continue;	
		}
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		auto Dest = endPos;
		auto RotateDirection = Dest - startPos;
		RotateDirection = FVector(RotateDirection.X, RotateDirection.Y, RotateDirection.Z);
		FRotator newrot = FRotationMatrix::MakeFromX(RotateDirection).Rotator();
		auto moduleNo = FMath::RandRange(0, 4);		
		auto module = ModuleList[moduleNo];

		FVector spawnPos = startPos;
		if (moduleNo == 0)
			spawnPos = endPos;

		auto spawnedModule = Cast<AEnemyModule>(GetWorld()->SpawnActor<AActor>(module, spawnPos, newrot, spawnParams));
		FAttachmentTransformRules attachRules(EAttachmentRule::KeepWorld, false);
		spawnedModule->AttachToActor(OwnerPawn, attachRules);
		spawnedModule->SetOwnerPawn(OwnerPawn);
		OwnerPawn->IncrementModuleCount();

	}
	//raycast in direction, if blocked, continue
	//else 
	//randomize a part, move into position, add to parent?
	//add to parent's counter
	//if exceeded size, break
}

void AEnemyModule::ActionFire()
{
	FVector startPos = GetActorLocation() + GetActorForwardVector() * 100;
	FVector endPos;
	
	endPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	
	FHitResult outHit;
	FCollisionQueryParams collisionParams;

	DrawDebugLine(GetWorld(), startPos, endPos, FColor::Red, true);
	
			//UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *outHit.GetActor()->GetName());
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	auto Dest = endPos;
	auto RotateDirection = Dest - startPos;
	RotateDirection = FVector(RotateDirection.X, RotateDirection.Y, RotateDirection.Z);
	FRotator newrot = FRotationMatrix::MakeFromX(RotateDirection).Rotator();
	//UE_LOG(LogTemp, Warning, TEXT("Rat - Newrot is %s and old rot is %s"),*newrot.ToString(), *ControlledPawn->GetActorRotation().ToString());
	//newrot.Roll = ControlledCharacter->GetActorRotation().Roll;
	TSubclassOf<AActor> projectile;
	if (ModuleType == EModuleTypes::S_Gun)
		projectile = GunProjectile;
	else
		projectile = MissileProjectile;
	GetWorld()->SpawnActor<AActor>(projectile, startPos, newrot, spawnParams);
	
}


void AEnemyModule::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->ActorHasTag(TEXT("CharProjectile")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Ouch"));
			Health -= 10;
			if (Health <= 0)
			{
				IsModuleActive = false;
				FDetachmentTransformRules detachRules(EDetachmentRule::KeepRelative,false);
				DetachFromActor(detachRules);
				FindComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(true);
			}
			OtherActor->Destroy();
		}
	}
}
void AEnemyModule::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}

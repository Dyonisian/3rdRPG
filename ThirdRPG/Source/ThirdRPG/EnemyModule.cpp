// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyModule.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "EnemyPawn.h"
#include "TimerManager.h"

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
	if (ModuleType == EModuleTypes::S_Weak)
	{
		Health = MaxHealth / 4;
	}
	FlashTimer = 0.0f;
	IsFlashing = false;
}
// Called every frame
void AEnemyModule::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FlashTimer -= GetWorld()->GetDeltaSeconds();
	if (FlashTimer <= 0.0f && IsFlashing)
	{
		ResetMaterial();
		IsFlashing = false;
		FlashTimer = 0.0f;
	}

	if (!IsModuleActive)
		return;

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
			{
				StateTimer = GunCooldown * 3;
				GunBurstCount = 0;
			}
			ActionFire();
			break;
		}
	}	
}

void AEnemyModule::SetHealthToZero()
{
	Health = 0;
	FlashRed();
	OnZeroHealth();
}

void AEnemyModule::AddModules()
{
	//Check list
	//loop
	for (int i = 0; i<ModulePositions.Num(); i++)
	{
		///Check parent max modules, if exceeded, return
		if (OwnerPawn->GetModuleCount() >= OwnerPawn->GetMaxModules())
			break;
		auto pos = ModulePositions[i];
				
		FVector startPos = GetActorLocation() + pos * 170;
		FVector endPos;
		endPos = GetActorLocation() + pos * 250;
		FHitResult outHit;
		FCollisionQueryParams collisionParams;

		//DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, true);
		//Check if there's space for a new module
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
}

void AEnemyModule::ActionFire()
{
	FVector endPos;
	endPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	if (FVector::DistSquared(GetActorLocation(), endPos) > AttackRange * AttackRange)
		return;

	FVector startPos = GetActorLocation() + GetActorForwardVector() * 100;
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;

	//Set rotation to face target
	auto Dest = endPos;
	auto RotateDirection = Dest - startPos;
	RotateDirection = FVector(RotateDirection.X, RotateDirection.Y, RotateDirection.Z);
	FRotator newrot = FRotationMatrix::MakeFromX(RotateDirection).Rotator();
	
	TSubclassOf<AActor> projectile;
	if (ModuleType == EModuleTypes::S_Gun)
		projectile = GunProjectile;
	else
		projectile = MissileProjectile;

	GetWorld()->SpawnActor<AActor>(projectile, startPos, newrot, spawnParams);	
}

void AEnemyModule::OnZeroHealth()
{
	IsModuleActive = false;
	
	FindComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(true);
	if (ModuleType == EModuleTypes::S_Weak)
		OnDestroyEvent();
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AEnemyModule::DestroySelf, 5.0f, false);
	FDetachmentTransformRules detachRules(EDetachmentRule::KeepRelative, false);
	DetachFromActor(detachRules);
}

void AEnemyModule::DestroySelf()
{
	Destroy();
}


void AEnemyModule::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && OtherComp)
	{
		if (!IsModuleActive)
			return;
		if (OtherActor->ActorHasTag(TEXT("CharProjectile")))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Ouch"));
			Health -= 10;
			FlashTimer = FlashCooldown;
			IsFlashing = true;
			FlashRed();
			if (Health <= 0)
			{
				OnZeroHealth();
			}
			OtherActor->Destroy();
		}
		if (OtherActor->ActorHasTag(TEXT("CharExplosion")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Explosion damage!"));
			Health -= 100;
			if (Health <= 0)
			{
				OnZeroHealth();
			}
		}
		if (OtherActor->ActorHasTag(TEXT("TrapExplosionChar")))
		{
			FlashTimer = FlashCooldown;
			IsFlashing = true;
			FlashRed();
			Health -= 50;
			if (Health <= 0)
			{
				OnDestroyEvent();
			}
		}
	}
}
void AEnemyModule::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}

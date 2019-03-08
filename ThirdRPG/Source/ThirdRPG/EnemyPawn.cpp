// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyPawn.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "EnemyModule.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsHandleComponent.h"



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
	FlashTimer = ExplosionImmunityTimer = 0.0f;
	IsFlashing = false;
	PhysicsHandleCount = 0;
	//auto t = TEXT("Physics Handle");// +static_cast<TEXT>(PhysicsHandleCount);	
	//SM1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Debug component"));
	for (; PhysicsHandleCount < 6; )
	{
		PhysicsHandles.Add(CreateDefaultSubobject<UPhysicsHandleComponent>(*FString("Physics Handle" + FString::FromInt(PhysicsHandleCount++))));
	}


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
	MaxModules = FMath::RandRange(128, 1024);
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

		FVector startPos = GetActorLocation() + FVector::UpVector * 400 + pos * 300;
		FVector endPos;		
		endPos = GetActorLocation() + FVector::UpVector * 400 + pos * 380;
		FHitResult outHit;
		FCollisionQueryParams collisionParams;

		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, true);
		//Check if space is already occupied by an object or another module
		if (GetWorld()->LineTraceSingleByChannel(outHit, startPos, endPos, ECC_Visibility, collisionParams))
		{
			if (outHit.bBlockingHit)
			{
				UE_LOG(LogTemp, Warning, TEXT("Can't build, something's in the way"));
				continue;
			}			
		}
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		auto dest = endPos;
		auto rotateDirection = dest - startPos;
		rotateDirection = FVector(rotateDirection.X, rotateDirection.Y, rotateDirection.Z);
		FRotator newrot = FRotationMatrix::MakeFromX(rotateDirection).Rotator();
		//Larger chance for ModuleHolder parts to allow for large enemies
		auto moduleNo = 1;//FMath::RandRange(1, 4);
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
		//auto t = TEXT("Physics Handle %i" + PhysicsHandleCount);
		//PhysicsHandles.Add(CreateDefaultSubobject<UPhysicsHandleComponent>(t));
		auto moduleComp = spawnedModule->CollisionComponent; //Cast<UPrimitiveComponent>(spawnedModule->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		PhysicsHandles[i]->GrabComponent(moduleComp, TEXT(""),spawnPos,true);
		//UE_LOG(LogTemp, Warning, TEXT("Got to this point"));
		SpawnedModulePositions.Add(pos * 300);
		SpawnedModuleRotators.Add(newrot - CollisionComponent->GetComponentRotation());
		ModuleCount++;
	}
	
}

// Called every frame
void AEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//DrawDebugLine(GetWorld(), GetActorLocation() + ModulePositions[1] * 170 , GetActorLocation() + ModulePositions[1] * 250, FColor::Blue, true);
	FlashTimer -= GetWorld()->GetDeltaSeconds();
	ExplosionImmunityTimer -= GetWorld()->GetDeltaSeconds();

	if (FlashTimer <= 0.0f && IsFlashing)
	{
		ResetMaterial();
		IsFlashing = false;
		FlashTimer = 0.0f;
	}
	for (int i=0; i<PhysicsHandles.Num()-1;i++)
	{
		PhysicsHandles[i]->SetTargetLocation(CollisionComponent->GetComponentLocation() + SpawnedModulePositions[i]);
		PhysicsHandles[i]->SetTargetRotation(CollisionComponent->GetComponentRotation() + SpawnedModuleRotators[i]);
	}

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
			Health -= 10;
			FlashTimer = FlashCooldown;
			IsFlashing = true;
			FlashRed();
			if (Health <= 0)
			{
				OnDestroyEvent();
				OtherActor->Destroy();
			}
		}
		if (OtherActor->ActorHasTag(TEXT("CharExplosion")))
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
		if (OtherActor->ActorHasTag(TEXT("TrapExplosionChar")) && ExplosionImmunityTimer<=0.0f)
		{
			FlashTimer = FlashCooldown;
			IsFlashing = true;
			ExplosionImmunityTimer = ExplosionImmunityCooldown;
			FlashRed();
			CollisionComponent->SetSimulatePhysics(true);
			CollisionComponent->SetEnableGravity(true);
			auto imp = (GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal();
			CollisionComponent->AddImpulseAtLocation(imp * 5000000,GetActorLocation() + (OtherActor->GetActorLocation() - GetActorLocation())/2);
			Health -= 50;
			if (Health <= 0)
			{
				OnDestroyEvent();
			}			
		}
	}
}
void AEnemyPawn::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}



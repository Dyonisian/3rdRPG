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
	FlashTimer = 0.0f;
	IsFlashing = false;

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
	MaxModules = FMath::RandRange(512, 4096);
	AddModules();
	SizeX = SizeY = SizeZ = 5;
	SizeZ = 2;
	//ModuleGrid.Init(TArray<TArray<int>()>(0,SizeZ), SizeX);
	TArray<int> Z;
	Z.Init(0, SizeZ);
	TArray < TArray<int>> Y;
	Y.Init(Z,SizeY);
	ModuleGrid.Init(Y, SizeX);
	//GenerateGrid();
	UE_LOG(LogTemp, Warning, TEXT("Size: %i, %i, %i"), ModuleGrid.Num(), ModuleGrid[0].Num(), ModuleGrid[0][0].Num());

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

		FVector startPos = GetActorLocation() + FVector::UpVector * 500 + pos * 170;
		FVector endPos;		
		endPos = GetActorLocation() + FVector::UpVector * 500 + pos * 250;
		FHitResult outHit;
		FCollisionQueryParams collisionParams;

		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, false, 1.0f);
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
	
}

void AEnemyPawn::GenerateGrid()
{
	for (int i = 0; i < SizeX; i++)
	{
		for (int j = 0; j < SizeY; j++)
		{
			for (int k = 0; k < SizeZ; k++)
			{
				//Bias the randomness
				ModuleGrid[i][j][k] = FMath::RandRange(0, 4);
				if (ModuleGrid[i][j][k] == 0)
					continue;

				FVector startPos = GetActorLocation() + FVector::UpVector * (SizeZ-k) * 300 - FVector::ForwardVector * (SizeX/2-i) * 300- FVector::RightVector * (SizeY/2-j) * 300;

				FActorSpawnParameters spawnParams;
				spawnParams.Owner = this;
				//Rotation
				
				auto spawnedModule = Cast<AEnemyModule>(GetWorld()->SpawnActor<AActor>(ModuleList[ModuleGrid[i][j][k]], startPos, FRotator(), spawnParams));
				FAttachmentTransformRules attachRules(EAttachmentRule::KeepWorld, false);
				if (spawnedModule)
				{
					spawnedModule->AttachToActor(this, attachRules);
					spawnedModule->SetOwnerPawn(this);
				}
				ModuleCount++;
			}
		}
	}
}

// Called every frame
void AEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//DrawDebugLine(GetWorld(), GetActorLocation() + ModulePositions[1] * 170 , GetActorLocation() + ModulePositions[1] * 250, FColor::Blue, true);
	FlashTimer -= GetWorld()->GetDeltaSeconds();
	if (FlashTimer <= 0.0f && IsFlashing)
	{
		ResetMaterial();
		IsFlashing = false;
		FlashTimer = 0.0f;
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
void AEnemyPawn::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}



// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyModule.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"


// Sets default values
AEnemyModule::AEnemyModule()
{
	ModuleType = EModuleTypes::S_Holder;
	PrimaryActorTick.bCanEverTick = true;
	IsModuleActive = false;
	StateTimer = 0.0f;
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

void AEnemyModule::AddModules()
{
}

void AEnemyModule::ActionFire()
{

}

// Called every frame
void AEnemyModule::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	StateTimer -= GetWorld()->GetDeltaSeconds();
	if (StateTimer <= 0.0f)
	{
		switch (ModuleType)
		{
		case EModuleTypes::S_Gun: StateTimer = GunCooldown;
		case EModuleTypes::S_Missile: StateTimer = MissileCooldown;
			ActionFire();
			break;
		}
	}
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
				Destroy();
			}
			OtherActor->Destroy();
		}
	}
}
void AEnemyModule::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}

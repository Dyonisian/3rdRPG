// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingManager.h"
#include "ThirdRPGCharacter.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BuildingBase.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UBuildingManager::UBuildingManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	XGridSize = YGridSize = ZGridSize = 400.0f;
	RotSnap = 90.0f;
	// ...
}


// Called when the game starts
void UBuildingManager::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<AThirdRPGCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	// ...
	
}

void UBuildingManager::PreviewFloor()
{
}

void UBuildingManager::PreviewWall()
{
	

}

void UBuildingManager::PreviewRamp()
{
}

FVector UBuildingManager::ToGridLocation(FVector loc)
{
	float x, y, z;
	x = (FMath::RoundToFloat(loc.X / XGridSize)) * XGridSize;
	y = (FMath::RoundToFloat(loc.Y / YGridSize)) * YGridSize;
	z = (FMath::RoundToFloat(loc.Z / ZGridSize)) * ZGridSize;

	return FVector(x,y,z);
}

float UBuildingManager::ToGridRotation(float rot)
{
	return (FMath::RoundToFloat(rot/RotSnap))*RotSnap;
}

FVector UBuildingManager::GetPlayerLookAtLocation(float offset)
{
	return PlayerCharacter->GetActorLocation() + PlayerCharacter->GetFollowCamera()->GetForwardVector() * offset - FVector::UpVector * 140;
}


// Called every frame
void UBuildingManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CurrentBuildPreview)
	{
		CurrentBuildPreview->SetActorLocation(ToGridLocation(GetPlayerLookAtLocation(400.0f)));
		CurrentBuildPreview->SetActorRotation(FRotator(0, ToGridRotation(PlayerCharacter->GetFollowCamera()->GetComponentRotation().Yaw+90),0));
	}
	// ...
}

void UBuildingManager::PreviewBuildType(EBuildingTypes buildType)
{
	if (CurrentBuildPreview!=nullptr)
	{
		//GetOwner()->Destroy(CurrentBuildPreview);
		CurrentBuildPreview->Destroy();
		CurrentBuildPreview = nullptr;
	}
	BuildType = buildType;
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = GetOwner();
	FVector startPos = ToGridLocation(GetPlayerLookAtLocation(400.0f));
	FRotator startRot = FRotator(0,ToGridRotation(PlayerCharacter->GetFollowCamera()->GetComponentRotation().Yaw +90),0);
	UE_LOG(LogTemp, Warning, TEXT("Rotator is %f %f %f"), startRot.Pitch, startRot.Roll, startRot.Yaw);

	if (BuildPreviewList[(int)buildType])
	{
		CurrentBuildPreview = GetWorld()->SpawnActor<ABuildingBase>(BuildPreviewList[(int)buildType], startPos, startRot, spawnParams);		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BuildPreviewList issue!"));
	}
}

void UBuildingManager::Build()
{
	if (CurrentBuildPreview)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = GetOwner();
		//FVector startPos = ToGridLocation(GetPlayerLookAtLocation(400.0f));
		//FRotator startRot = FRotator(0, ToGridRotation(PlayerCharacter->GetFollowCamera()->GetComponentRotation().Yaw + 90), 0);
		FVector startPos = CurrentBuildPreview->GetActorLocation();
		FRotator startRot = CurrentBuildPreview->GetActorRotation();
		if (BuildList[(int)BuildType])
		{
			GetWorld()->SpawnActor<ABuildingBase>(BuildList[(int)BuildType], startPos, startRot, spawnParams);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("BuildList issue!"));
		}
	}

}

void UBuildingManager::CancelBuildMode()
{
	if (CurrentBuildPreview)
	{
		CurrentBuildPreview->Destroy();
		CurrentBuildPreview = nullptr;
	}
}


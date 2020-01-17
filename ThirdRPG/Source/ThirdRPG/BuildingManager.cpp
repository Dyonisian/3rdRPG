// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingManager.h"
#include "ThirdRPGCharacter.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BuildingBase.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
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
		if (BuildType == EBuildingTypes::S_Edit)
		{
			FVector startPos = GetPlayerLookAtLocation(100) + FVector::UpVector * 200;//PlayerCharacter->GetFollowCamera()->GetComponentLocation() + PlayerCharacter->GetFollowCamera()->GetForwardVector() * 170;
			FVector endPos = GetPlayerLookAtLocation(400) + FVector::UpVector * 200;
			FHitResult outHit;
			FCollisionQueryParams collisionParams;
			//Check if space is already occupied by a build object
			if (GetWorld()->LineTraceSingleByChannel(outHit, startPos, endPos, ECC_Visibility, collisionParams))
			{
				CurrentBuildPreview->SetActorLocation(outHit.ImpactPoint);
			}
			else
			{
				CurrentBuildPreview->SetActorLocation(GetPlayerLookAtLocation(400.0f) + FVector::UpVector * 200);
			}
		}
		else
		{
			CurrentBuildPreview->SetActorLocation(ToGridLocation(GetPlayerLookAtLocation(400.0f)));
			CurrentBuildPreview->SetActorRotation(FRotator(0, ToGridRotation(PlayerCharacter->GetFollowCamera()->GetComponentRotation().Yaw + 90), 0));
		}
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
	//UE_LOG(LogTemp, Warning, TEXT("Rotator is %f %f %f"), startRot.Pitch, startRot.Roll, startRot.Yaw);

	if (BuildPreviewList.Num()>(int)BuildType)
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
		if (BuildList.Num()>(int)BuildType)
		{
			if (BuildType == EBuildingTypes::S_Edit)
			{
				FVector startPos = GetPlayerLookAtLocation(100) + FVector::UpVector * 200;//PlayerCharacter->GetFollowCamera()->GetComponentLocation() + PlayerCharacter->GetFollowCamera()->GetForwardVector() * 170;
				FVector endPos = GetPlayerLookAtLocation(400) + FVector::UpVector * 200;
				//endPos = startPos + PlayerCharacter->GetFollowCamera()->GetForwardVector() * 400;
				FHitResult outHit;
				FCollisionQueryParams collisionParams;

				DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, false, 1.0f);
				//Check if space is already occupied by a build object
				if (GetWorld()->LineTraceSingleByChannel(outHit, startPos, endPos, ECC_Visibility, collisionParams))
				{
					auto actor = outHit.GetActor();
					if (outHit.bBlockingHit && actor->ActorHasTag("Build"))
					{
						auto pos = actor->GetActorLocation();
						auto rot = actor->GetActorRotation();
						//Check which half the hit is in 
						auto diffPos = actor->GetTransform().InverseTransformPosition(endPos);//Convert to local space of the hit object (Build object)
						//UE_LOG(LogTemp, Warning, TEXT("Diff pos is %s"), *diffPos.ToString());
						if (diffPos.X < 200)
							rot.Yaw += 180;
						if (diffPos.Z > 200)
							rot.Roll += 180;

						//Replace mesh
						pos += actor->GetActorUpVector() * EditOffsetZ + actor->GetActorRightVector() * EditOffsetX + actor->GetActorForwardVector() * EditOffsetY;						
						FActorSpawnParameters spawnParams;
						spawnParams.Owner = GetOwner();
						actor->Destroy();
						GetWorld()->SpawnActor<ABuildingBase>(BuildList[(int)BuildType], pos, rot, spawnParams);
					}
				}
			}
			//Normal build, not edit
			else
			{
				FActorSpawnParameters spawnParams;
				spawnParams.Owner = GetOwner();
				//FVector startPos = ToGridLocation(GetPlayerLookAtLocation(400.0f));
				//FRotator startRot = FRotator(0, ToGridRotation(PlayerCharacter->GetFollowCamera()->GetComponentRotation().Yaw + 90), 0);
				FVector startPos = CurrentBuildPreview->GetActorLocation();
				FRotator startRot = CurrentBuildPreview->GetActorRotation();
				GetWorld()->SpawnActor<ABuildingBase>(BuildList[(int)BuildType], startPos, startRot, spawnParams);
			}
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


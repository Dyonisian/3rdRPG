// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ThirdRPGGameMode.h"
#include "ThirdRPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThirdRPGGameMode::AThirdRPGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

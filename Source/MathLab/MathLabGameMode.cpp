// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "MathLab.h"
#include "MathLabGameMode.h"
#include "MathLabHUD.h"
#include "MathLabCharacter.h"

AMathLabGameMode::AMathLabGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/MathLab/Blueprints/Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AMathLabHUD::StaticClass();
}

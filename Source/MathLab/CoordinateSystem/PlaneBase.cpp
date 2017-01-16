// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "PlaneBase.h"

APlaneBase::APlaneBase()
{
}

// Unreal Events -----------------------------------------------------------------------------------

void APlaneBase::BeginPlay()
{
   Super::BeginPlay();
}

// Initialise --------------------------------------------------------------------------------------

void APlaneBase::SetComponents(UStaticMeshComponent *plane, TArray<UStaticMeshComponent *> laserComponents)
{
}

// Update -------------------------------------------------------------------------------------------

void APlaneBase::Update()
{
   Super::Update();
}

// -------------------------------------------------------------------------------------------------

void APlaneBase::SetValuesPlane(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector position, FVector direction1, FVector direction2)
{
}

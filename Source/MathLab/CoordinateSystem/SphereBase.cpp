// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "SphereBase.h"

ASphereBase::ASphereBase()
{
}

// Unreal Events -----------------------------------------------------------------------------------

void ASphereBase::BeginPlay()
{
   Super::BeginPlay();
}

// Initialise --------------------------------------------------------------------------------------

void ASphereBase::SetComponents(UStaticMeshComponent* sphere, TArray<UStaticMeshComponent*> laserComponents)
{
}

// Update -------------------------------------------------------------------------------------------

void ASphereBase::Update()
{
   Super::Update();
}

// -------------------------------------------------------------------------------------------------

void ASphereBase::SetValuesSphere(ACoordinateSystemBase* coordinateSystem, LaserColors color, FVector coordinate, float radius)
{
}

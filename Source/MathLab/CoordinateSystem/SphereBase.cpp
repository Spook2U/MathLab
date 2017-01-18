// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "SphereBase.h"
#include "PointBase.h"
#include "LineBase.h"

ASphereBase::ASphereBase()
{
   Radius = 0.f;
}

// Unreal Events -----------------------------------------------------------------------------------

void ASphereBase::BeginPlay()
{
   Super::BeginPlay();
}

// Initialise --------------------------------------------------------------------------------------

void ASphereBase::SetComponents(UStaticMeshComponent* sphere, TArray<UStaticMeshComponent*> laserComponents)
{
   this->Sphere = sphere;
   Sphere->SetScalarParameterValueOnMaterials("Transparency", 0.1f);

   for(UStaticMeshComponent *l : laserComponents)
   {
      LaserCompoents.Add(l);
   }
}

// Update -------------------------------------------------------------------------------------------

void ASphereBase::Update()
{
   Super::Update();
   BuildSphere();
}

// Setup --------------------------------------------------------------------------------------------

void ASphereBase::BuildSphere()
{
   Sphere->SetWorldScale3D(FVector(1, 1, 1) * (Radius*CoordinateSystem->ConvertFactor/100));
}

// -------------------------------------------------------------------------------------------------

void ASphereBase::SetValuesSphere(ACoordinateSystemBase* coordinateSystem, LaserColors color, FVector coordinate, float radius)
{
   SetValuesPoint(coordinateSystem, color, coordinate);
   this->Radius = radius;
   CreateGuides(color);
}

// Protected ----------------------------------------------------------------------------------------

void ASphereBase::CreateGuides(LaserColors color)
{
   if(IsGuide) return;

   AddGuide(CoordinateSystem->AddPoint(color, true, Coordinate));

   AddGuide(CoordinateSystem->AddLine(color, true, Coordinate, Coordinate + FVector(Radius, 0, 0), LineMode::segment));
}



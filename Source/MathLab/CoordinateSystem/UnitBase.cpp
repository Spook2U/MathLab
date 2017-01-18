// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "UnitBase.h"

void AUnitBase::BeginPlay() { Super::BeginPlay(); }

// Initialise --------------------------------------------------------------------------------------

void AUnitBase::SetComponents(UStaticMeshComponent *xAxis, UStaticMeshComponent *yAxis, UStaticMeshComponent *xLaser, UStaticMeshComponent *yLaser, TArray<UStaticMeshComponent *> laserComponents)
{
   this->XAxis  = xAxis;
   this->YAxis  = yAxis;
   this->XLaser = xLaser;
   this->YLaser = yLaser;

   for(UStaticMeshComponent *l : laserComponents)
   {
      LaserCompoents.Add(l);
   }
}

// Update -------------------------------------------------------------------------------------------

void AUnitBase::Update()
{
   Super::Update();
   
   ScaleUnitLaser();
}

// Setup --------------------------------------------------------------------------------------------

void AUnitBase::ScaleUnitLaser()
{
   MLD_PTR_CHECK(CoordinateSystem);
   if(CoordinateSystem)
   {
      ScaleUnitLaser_AtAxis(XAxis, XLaser, CoordinateSystem->LaserSizeFactor);
      ScaleUnitLaser_AtAxis(YAxis, YLaser, CoordinateSystem->LaserSizeFactor);
   }
}

void AUnitBase::ScaleUnitLaser_AtAxis(UStaticMeshComponent *axis, UStaticMeshComponent *laser, float laserSize)
{
   FVector axisScale = axis->GetComponentScale();

   laser->SetWorldScale3D(FVector(axisScale.X*laserSize, axisScale.Y*laserSize, CoordinateSystem->AxisLength*2));
}

// -------------------------------------------------------------------------------------------------

void AUnitBase::OrientateToAxis(UStaticMeshComponent *axis)
{
   MLD_PTR_CHECK(CoordinateSystem);
   if(CoordinateSystem)
   {
      float thickness = CoordinateSystem->AxisSize*CoordinateSystem->UnitSizeFactor;

      FRotator rotation = axis->GetComponentRotation();
      FVector  location = GetActorLocation();
      FVector  scale = FVector(thickness, thickness, 0.1f);

      SetActorTransform(FTransform(rotation, location, scale));
   }
}

// -------------------------------------------------------------------------------------------------

void AUnitBase::CreateGuides(LaserColors color)
{
   //leave empty to override from Point, so Units dont create Guides
}

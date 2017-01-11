// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "UnitBase.h"

void AUnitBase::BeginPlay() { Super::BeginPlay(); }



void AUnitBase::SetComponents(UStaticMeshComponent *xAxis, UStaticMeshComponent *yAxis, UStaticMeshComponent *xLaser, UStaticMeshComponent *yLaser)
{
   this->XAxis  = xAxis;
   this->YAxis  = yAxis;
   this->XLaser = xLaser;
   this->YLaser = yLaser;
}



void AUnitBase::Update()
{
   Super::Update();
   
   ScaleUnitLaser();
}



void AUnitBase::ScaleUnitLaser()
{
   POINTERTEST(CoordinateSystem);
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



void AUnitBase::OrientateToAxis(UStaticMeshComponent *axis)
{
   POINTERTEST(CoordinateSystem);
   if(CoordinateSystem)
   {
      float thickness = CoordinateSystem->AxisSize*CoordinateSystem->UnitSizeFactor;

      FRotator rotation = axis->GetComponentRotation();
      FVector  location = GetActorLocation();
      FVector  scale = FVector(thickness, thickness, 0.1f);

      SetActorTransform(FTransform(rotation, location, scale));
   }
}

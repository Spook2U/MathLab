// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "UnitBase.h"

void AUnitBase::SetAxis(UStaticMeshComponent * xAxis, UStaticMeshComponent * yAxis, UStaticMeshComponent * xLaser, UStaticMeshComponent * yLaser)
{
   this->XAxis  = xAxis;
   this->YAxis  = yAxis;
   this->XLaser = xLaser;
   this->YLaser = yLaser;
}

void AUnitBase::OrientateToAxis(UStaticMeshComponent *axis)
{
   float thickness = CoordinateSystem->AxisSize*CoordinateSystem->UnitSizeFactor;

   FRotator rotation = axis->GetComponentRotation();
   FVector  location = GetActorLocation();
   FVector  scale    = FVector(thickness, thickness, 0.1f);

   SetActorTransform(FTransform(rotation, location, scale));
}

void AUnitBase::ScaleUnitLaser()
{
   ScaleUnitLaser_AtAxis(XAxis, XLaser, CoordinateSystem->LaserSizefactor);
   ScaleUnitLaser_AtAxis(YAxis, YLaser, CoordinateSystem->LaserSizefactor);
}

void AUnitBase::ScaleUnitLaser_AtAxis(UStaticMeshComponent *laser, UStaticMeshComponent *axis, float laserSize)
{
   FVector axisScale = axis->GetComponentScale();

   laser->SetWorldScale3D(FVector(axisScale.X*laserSize, axisScale.Y*laserSize, CoordinateSystem->AxisLength*2));
}

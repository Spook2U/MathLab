// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "UnitBase.h"

AUnitBase::AUnitBase()
{
   XAxis  = nullptr;
   YAxis  = nullptr;
   XLaser = nullptr;
   YLaser = nullptr;
}

void AUnitBase::BeginPlay() 
{ 
   Super::BeginPlay();
}



void AUnitBase::SetComponents(TArray<UStaticMeshComponent *> components)
{
   for(UStaticMeshComponent *c : components)
   {
      MLD_PTR_CHECK(c); if(!c) continue;
      if(c->GetName().Equals("X-Unit"))  { this->XAxis  = c; }
      if(c->GetName().Equals("Y-Unit"))  { this->YAxis  = c; }
      if(c->GetName().Equals("X-Laser")) { this->XLaser = c; }
      if(c->GetName().Equals("Y-Laser")) { this->YLaser = c; }
   }

   MLD_PTR_CHECK(XAxis);
   MLD_PTR_CHECK(YAxis);
   MLD_PTR_CHECK(XLaser);
   MLD_PTR_CHECK(YLaser);
   if(!(XAxis && YAxis && XLaser && YLaser)) return;

   AddLaserComponent(XLaser);
   AddLaserComponent(YLaser);
}

void AUnitBase::InitUnit(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector coordinate)
{
   MLD_PTR_CHECK(coordinateSystem); if(!coordinateSystem) return;
   FPoint p;
   p.Coordinate = coordinate;
   InitPoint(coordinateSystem, color, p);
   this->type = GeometryType::unit; //after InitPoint() to override the value from point
}



void AUnitBase::Update()
{
   Super::Update();
   ScaleUnitLaser();
}



void AUnitBase::ScaleUnitLaser()
{
   ScaleUnitLaser_AtAxis(XAxis, XLaser, CoordinateSystem->LaserSizeFactor);
   ScaleUnitLaser_AtAxis(YAxis, YLaser, CoordinateSystem->LaserSizeFactor);
}

void AUnitBase::ScaleUnitLaser_AtAxis(UStaticMeshComponent *axis, UStaticMeshComponent *laser, float laserSize)
{
   FVector axisScale = axis->GetComponentScale();
   laser->SetWorldScale3D(FVector(axisScale.X*laserSize, axisScale.Y*laserSize, CoordinateSystem->AxisLength*2));
}



void AUnitBase::OrientateToAxis(UStaticMeshComponent *axis)
{
   float thickness = CoordinateSystem->AxisSize*CoordinateSystem->UnitSizeFactor;
   SetActorTransform(FTransform(axis->GetComponentRotation(), GetActorLocation(), FVector(thickness, thickness, 0.1f)));
}

// -------------------------------------------------------------------------------------------------

void AUnitBase::CreateVectorGuides(LaserColors color)
{
   //leave empty to override from Point, so Units dont create Guides
}

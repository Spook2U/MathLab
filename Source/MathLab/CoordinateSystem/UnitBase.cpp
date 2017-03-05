// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "UnitBase.h"

#include "CoordinateSystemBase.h"



AUnitBase::AUnitBase()
{
   xAxis  = nullptr;
   yAxis  = nullptr;
   xLaser = nullptr;
   yLaser = nullptr;
}

void AUnitBase::BeginPlay() 
{ 
   Super::BeginPlay();
}



void AUnitBase::SetComponents(TArray<UStaticMeshComponent *> components, UTextRenderComponent *inText)
{
   for(UStaticMeshComponent *c : components)
   {
      MLD_PTR_CHECK(c); if(!c) continue;
      if(c->GetName().Equals("X-Unit"))  { xAxis  = c; }
      if(c->GetName().Equals("Y-Unit"))  { yAxis  = c; }
      if(c->GetName().Equals("X-Laser")) { xLaser = c; }
      if(c->GetName().Equals("Y-Laser")) { yLaser = c; }
   }

   MLD_PTR_CHECK(xAxis);
   MLD_PTR_CHECK(yAxis);
   MLD_PTR_CHECK(xLaser);
   MLD_PTR_CHECK(yLaser);
   if(!(xAxis && yAxis && xLaser && yLaser)) return;

   AddLaserComponent(xLaser);
   AddLaserComponent(yLaser);

   if(!MLD_PTR_CHECK(inText)) return;
   nameText = inText;
}

void AUnitBase::Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FVector inCoordinate, FString inName)
{
   MLD_PTR_CHECK(inCoordinateSystem); if(!inCoordinateSystem) return;

   type = GeometryType::unit;
   coordinate = inCoordinate;

   Super::Init(inCoordinateSystem, inColor, inName);
   InitText(inName);
}



void AUnitBase::Update()
{
   Super::Update();
   SetPosition(coordinate);
   ScaleUnitLaser();
}



void AUnitBase::ScaleUnitLaser()
{
   ScaleUnitLaser_AtAxis(xAxis, xLaser, coordinateSystem->laserSizeFactor);
   ScaleUnitLaser_AtAxis(yAxis, yLaser, coordinateSystem->laserSizeFactor);
}

void AUnitBase::ScaleUnitLaser_AtAxis(UStaticMeshComponent *axis, UStaticMeshComponent *laser, float laserSize)
{
   FVector axisScale = axis->GetComponentScale();
   laser->SetWorldScale3D(FVector(axisScale.X*laserSize, axisScale.Y*laserSize, coordinateSystem->axisLength*2));
}



void AUnitBase::OrientateToAxis(UStaticMeshComponent *axis)
{
   float thickness = coordinateSystem->axisSize*coordinateSystem->unitSizeFactor;
   SetActorTransform(FTransform(axis->GetComponentRotation(), GetActorLocation(), FVector(thickness, thickness, 0.1f)));
   nameText->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
}

// -------------------------------------------------------------------------------------------------

void AUnitBase::CreateCVector(LaserColors inColor)
{}

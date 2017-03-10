// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "UnitBase.h"

#include "CoordinateSystemBase.h"



// Unit Class ----------------------------------------------------------------------------------------------------------------------------------------



AUnitBase::AUnitBase()
{
   xAxis  = nullptr;
   yAxis  = nullptr;
   xLaser = nullptr;
   yLaser = nullptr;
}

// Unreal Events -------------------------------------------------------------------------------------------------------------------------------------

void AUnitBase::BeginPlay() 
{ 
   Super::BeginPlay();
}

// Unit Setup ----------------------------------------------------------------------------------------------------------------------------------------

void AUnitBase::Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FVector inCoordinate, FName inName)
{
   MLD_PTR_CHECK(inCoordinateSystem); if(!inCoordinateSystem) return;

   type = GeometryType::unit;
   coordinate = inCoordinate;

   Super::Init(inCoordinateSystem, inColor, inName);
   InitName(inName);
   SetMathDataVisible(false);
}

void AUnitBase::SetComponents(TArray<UStaticMeshComponent *> components, UTextRenderComponent *inText)
{
   for(UStaticMeshComponent *c : components)
   {
      if(!MLD_PTR_CHECK(c)) continue;
      if(c->GetName().Equals("X-Unit"))  { xAxis  = c; }
      if(c->GetName().Equals("Y-Unit"))  { yAxis  = c; }
      if(c->GetName().Equals("X-Laser")) { xLaser = c; }
      if(c->GetName().Equals("Y-Laser")) { yLaser = c; }
   }

   if(!(MLD_PTR_CHECK(xAxis) && MLD_PTR_CHECK(yAxis) && MLD_PTR_CHECK(xLaser) && MLD_PTR_CHECK(yLaser) && MLD_PTR_CHECK(inText))) return;

   AddLaserComponent(xLaser);
   AddLaserComponent(yLaser);

   nameRender = inText;
}

void AUnitBase::OrientateToAxis(UStaticMeshComponent *axis)
{
   if(!MLD_PTR_CHECK(nameRender)) return;
   float thickness = coordinateSystem->axisSize*coordinateSystem->unitSizeFactor;
   SetActorTransform(FTransform(axis->GetComponentRotation(), GetActorLocation(), FVector(thickness, thickness, 0.1f)));
   nameRender->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
}

// Update Functions ----------------------------------------------------------------------------------------------------------------------------------

void AUnitBase::Update()
{
   Super::Update();
   Move(coordinate);
   ScaleUnitLaser();
}

void AUnitBase::ScaleUnitLaser()
{
   ScaleUnitLaser_AtAxis(xAxis, xLaser, coordinateSystem->unitLaserSizeFactor);
   ScaleUnitLaser_AtAxis(yAxis, yLaser, coordinateSystem->unitLaserSizeFactor);
}

void AUnitBase::ScaleUnitLaser_AtAxis(UStaticMeshComponent *axis, UStaticMeshComponent *laser, float laserSize)
{
   if(!MLD_PTR_CHECK(laser)) return;
   FVector axisScale = axis->GetComponentScale();
   laser->SetWorldScale3D(FVector(axisScale.X*laserSize, axisScale.Y*laserSize, coordinateSystem->axisLength*2));
}

// Setting Functions ---------------------------------------------------------------------------------------------------------------------------------

bool AUnitBase::SetName(FName inName)
{
   if(!MLD_PTR_CHECK(nameRender)) return false;
   nameRender->SetText(FText::FromName(BuildName(inName)));
   return true;
}

// Utility Functions----------------------------------------------------------------------------------------------------------------------------------

// Constructing Vector Functions ---------------------------------------------------------------------------------------------------------------------

void AUnitBase::CreateCVector(LaserColors inColor)
{}

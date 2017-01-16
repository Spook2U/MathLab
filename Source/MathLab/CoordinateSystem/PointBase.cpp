// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "PointBase.h"

APointBase::APointBase()
{
   Coordinate = FVector();
}

// Unreal Events -----------------------------------------------------------------------------------

void APointBase::BeginPlay() 
{ 
   Super::BeginPlay(); 
}

// Initialise --------------------------------------------------------------------------------------

void APointBase::SetComponents(UStaticMeshComponent *point, TArray<UStaticMeshComponent *> laserComponents)
{
   this->Point = point;
   
   Point->SetWorldScale3D(Point->GetComponentScale()*Size);
  
   for(UStaticMeshComponent *l : laserComponents)
   {
      LaserCompoents.Add(l);
   }
}

// Update -------------------------------------------------------------------------------------------

void APointBase::Update()
{
   Super::Update();
   SetPosition(Coordinate);
}

// -------------------------------------------------------------------------------------------------

void APointBase::SetValuesPoint(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector coordinate)
{
   SetValues(coordinateSystem, color);
   this->Coordinate = coordinate;
   if(!IsGuide)
   {
      CreateGuidesPoint(color);
      ShowGuides(false);
   }
}

// Protected ----------------------------------------------------------------------------------------

void APointBase::CreateGuidesPoint(LaserColors color)
{
}

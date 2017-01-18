// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "PointBase.h"
#include "LineBase.h"

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

float APointBase::DistanceToLine(ALineBase * line)
{
   float distance = 0.0f;
   MLD_PTR_CHECK(line);
   if(line)
   {
      distance = UKismetMathLibrary::GetPointDistanceToLine(Coordinate, line->Position, line->Direction);
   }
   return distance;
}

float APointBase::DistanceToPoint(APointBase * point)
{
   float distance = 0.0f;
   MLD_PTR_CHECK(point);
   if(point)
   {
      distance = UKismetMathLibrary::VSize(point->Coordinate - Coordinate);
   }
   return distance;
}

float APointBase::DistanceToPlane(APlaneBase * plane)
{
   float distance = 0.0f;
   MLD_PTR_CHECK(plane);
   if(plane)
   {
      distance = 0.0f;
   }
   return distance;
}

float APointBase::DistanceToSphere(ASphereBase * sphere)
{
   return 0.0f;
}

// -------------------------------------------------------------------------------------------------

void APointBase::SetValuesPoint(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector coordinate)
{  
   
   SetValues(coordinateSystem, color);
   this->Coordinate = coordinate;
   CreateGuides(color);
}

// Protected ----------------------------------------------------------------------------------------

void APointBase::CreateGuides(LaserColors color)
{
   if(IsGuide) return;
   AddGuide(CoordinateSystem->AddLine(color, true, FVector(0, 0, 0), Coordinate, LineMode::vector));
}


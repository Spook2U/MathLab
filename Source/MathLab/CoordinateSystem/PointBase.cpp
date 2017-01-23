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



void APointBase::SetComponents(TArray<UStaticMeshComponent*> components)
{
   for(UStaticMeshComponent *c : components)
   {
      MLD_PTR_CHECK(c);
      if(c) { if(c->GetName().Equals("Point")) { this->Point = c; } }
   }
   
   MLD_PTR_CHECK(Point); if(!Point) return;
   ScalePointInit(Point);
   AddLaserComponent(Point);
}



void APointBase::SetValuesPoint(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector coordinate)
{  
   SetValues(coordinateSystem, color);
   this->Coordinate = coordinate;
   CreateGuides(color);
}



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



// Protected ----------------------------------------------------------------------------------------

void APointBase::CreateGuides(LaserColors color)
{
   if(IsGuide) return;
   AddGuide(CoordinateSystem->AddLine(color, true, FVector(0, 0, 0), Coordinate, LineMode::vector));
}


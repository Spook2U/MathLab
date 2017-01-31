// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "PointBase.h"
#include "LineBase.h"

APointBase::APointBase()
{
   Coordinate = FVector::ZeroVector;
   Point = nullptr;
}

void APointBase::BeginPlay() 
{ 
   Super::BeginPlay(); 
}



void APointBase::SetComponents(TArray<UStaticMeshComponent*> components)
{
   for(UStaticMeshComponent *c : components)
   {
      MLD_PTR_CHECK(c); if(!c) continue;
      if(c->GetName().Equals("Point")) { this->Point = c; }
   }
   
   MLD_PTR_CHECK(Point); if(!Point) return;
   InitScalePoint(Point);
   AddLaserComponent(Point);
}



void APointBase::InitPoint(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector coordinate)
{  
   MLD_PTR_CHECK(coordinateSystem); if(!coordinateSystem) return;

   SetValuesGeometry(coordinateSystem, color);
   this->Coordinate = coordinate;
   CreateVectorGuides(color);
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

void APointBase::CreateVectorGuides(LaserColors color)
{
   AddVectorGuide(CoordinateSystem->AddVectorStruct(color, FVector::ZeroVector, Coordinate, VectorStructMode::vector));
}


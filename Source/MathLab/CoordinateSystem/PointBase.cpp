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
   this->type = GeometryType::point;
   CreateVectorGuides(color);
}



void APointBase::Update()
{
   Super::Update();
   SetPosition(Coordinate);
}



// Protected ----------------------------------------------------------------------------------------

void APointBase::CreateVectorGuides(LaserColors color)
{
   AddVectorGuide(CoordinateSystem->AddVectorStruct(color, FVector::ZeroVector, Coordinate, VectorStructMode::vector));
}


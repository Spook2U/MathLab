// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "PointBase.h"

#include "CoordinateSystemBase.h"
#include "VectorStruct.h"



FMathPoint::FMathPoint() {}
FMathPoint::FMathPoint(FVector inCoordinate) : coordinate(inCoordinate) {}



APointBase::APointBase()
{
   point = FMathPoint();
   pointMesh = nullptr;
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
      if(c->GetName().Equals("PointMesh")) { this->pointMesh = c; }
   }
   
   MLD_PTR_CHECK(pointMesh); if(!pointMesh) return;
   InitScalePoint(pointMesh);
   AddLaserComponent(pointMesh);
}



void APointBase::InitPoint(ACoordinateSystemBase *inCoordinateSystem, LaserColors color, FMathPoint inPoint)
{  
   MLD_PTR_CHECK(inCoordinateSystem); if(!inCoordinateSystem) return;

   SetValuesGeometry(inCoordinateSystem, color);
   this->point = inPoint;
   this->type = GeometryType::point;
   CreateVectorGuides(color);
}



void APointBase::Update()
{
   Super::Update();
   SetPosition(point.coordinate);
}



// Protected ----------------------------------------------------------------------------------------

void APointBase::CreateVectorGuides(LaserColors color)
{
   AddVectorGuide(coordinateSystem->AddVectorStruct(color, FVector::ZeroVector, point.coordinate, VectorStructMode::vector));
}


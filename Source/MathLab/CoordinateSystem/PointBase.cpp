// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "PointBase.h"

#include "CoordinateSystemBase.h"
#include "VectorStruct.h"



FMathPoint::FMathPoint() {}
FMathPoint::FMathPoint(FVector inCoordinate) : coordinate(inCoordinate) {}

FString FMathPoint::ToString()
{
   return FString::Printf(TEXT("Coordinate %s"), *coordinate.ToString());
}

// -------------------------------------------------------------------------------------------------

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



void APointBase::Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FMathPoint inPoint)
{  
   MLD_PTR_CHECK(inCoordinateSystem); if(!inCoordinateSystem) return;
  
   Super::Init(inCoordinateSystem, inColor);

   this->point = inPoint;
   this->type = GeometryType::point;
   CreateVectorGuides(inColor);
}



void APointBase::Update()
{
   Super::Update();
   SetPosition(point.coordinate);
}

FString APointBase::ToString()
{
   return FString::Printf(TEXT("%s; %s"), *Super::ToString(), *point.ToString());
}



// Protected ----------------------------------------------------------------------------------------

void APointBase::CreateVectorGuides(LaserColors inColor)
{
   AddVectorGuide(coordinateSystem->AddVectorStruct(inColor, FVector::ZeroVector, point.coordinate, VectorStructMode::vector));
}


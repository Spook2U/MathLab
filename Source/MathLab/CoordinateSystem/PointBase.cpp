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



void APointBase::SetComponents(TArray<UStaticMeshComponent*> components, UTextRenderComponent *inText)
{
   for(UStaticMeshComponent *c : components)
   {
      MLD_PTR_CHECK(c); if(!c) continue;
      if(c->GetName().Equals("PointMesh")) { pointMesh = c; }
   }
   
   MLD_PTR_CHECK(pointMesh); if(!pointMesh) return;
   InitScalePoint(pointMesh);
   AddLaserComponent(pointMesh);

   if(!MLD_PTR_CHECK(inText)) return;
   nameText = inText;
}



void APointBase::Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FMathPoint inPoint, FText inName)
{  
   MLD_PTR_CHECK(inCoordinateSystem); if(!inCoordinateSystem) return;
  
   if(type == GeometryType::other) type = GeometryType::point; //to prevent override of Unit and Sphere
   Super::Init(inCoordinateSystem, inColor, inName);

   point = inPoint;
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


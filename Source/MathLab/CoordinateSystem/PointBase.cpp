// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "PointBase.h"

#include "CoordinateSystemBase.h"
#include "CVectorBase.h"



// Math Point Structure ------------------------------------------------------------------------------------------------------------------------------

FMathPoint::FMathPoint() {}
FMathPoint::FMathPoint(FVector inCoordinate) : coordinate(inCoordinate) {}

FMathPoint &FMathPoint::operator=(const FMathPoint &Other)
{
   this->coordinate = Other.coordinate;

   return *this;
}
bool FMathPoint::operator==(const FMathPoint &Other) const
{
   return (this->coordinate == Other.coordinate);
}
bool FMathPoint::operator!=(const FMathPoint &Other) const
{
   return !(this == &Other);
}

FString FMathPoint::ToString()
{
   return FString::Printf(TEXT("Coordinate %s"), *coordinate.ToString());
}

FString FMathPoint::ToStringShort()
{
   return FString::Printf(TEXT("(%s, %s, %s)"), *FString::SanitizeFloat(coordinate.X), *FString::SanitizeFloat(coordinate.Y), *FString::SanitizeFloat(coordinate.Z));
}



// Point Class ---------------------------------------------------------------------------------------------------------------------------------------


APointBase::APointBase()
{
   point = FMathPoint();
   pointMesh = nullptr;
}

// Unreal Events -------------------------------------------------------------------------------------------------------------------------------------

void APointBase::BeginPlay() 
{ 
   Super::BeginPlay(); 
}

// Point Setup ---------------------------------------------------------------------------------------------------------------------------------------

void APointBase::Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FMathPoint inPoint, FName inName)
{  
   if(!MLD_PTR_CHECK(inCoordinateSystem)) return;

   type = GeometryType::point;
   point = inPoint;
   nameMathData = FName(*inPoint.ToStringShort());
   Super::Init(inCoordinateSystem, inColor, inName);
   CreateCVector(inColor);
   InitName(inName);
}

void APointBase::SetComponents(TArray<UStaticMeshComponent*> components, UTextRenderComponent *inText)
{
   for(UStaticMeshComponent *c : components)
   {
      if(!MLD_PTR_CHECK(c)) continue;
      if(c->GetName().Equals("PointMesh")) { pointMesh = c; }
   }

   if(!(MLD_PTR_CHECK(pointMesh) && MLD_PTR_CHECK(inText))) return;
   InitScalePoint(pointMesh);
   AddLaserComponent(pointMesh);

   nameRender = inText;
}

// Update Functions ----------------------------------------------------------------------------------------------------------------------------------

void APointBase::Update()
{
   Super::Update();
   Move(point.coordinate);
}

// Setting Functions ---------------------------------------------------------------------------------------------------------------------------------

APointBase *APointBase::SetPoint(FMathPoint inPoint)
{
   point = inPoint;
   nameMathData = FName(*inPoint.ToStringShort());

   if(!MLD_PTR_CHECK(constVectors[0])) return nullptr;
   constVectors[0]->SetCVector(FVector::ZeroVector, point.coordinate);

   Update();

   return this;
}

// Utility Functions----------------------------------------------------------------------------------------------------------------------------------

FString APointBase::ToString()
{
   return FString::Printf(TEXT("%s; %s"), *Super::ToString(), *point.ToString());
}

// Constructing Vector Functions ---------------------------------------------------------------------------------------------------------------------

void APointBase::CreateCVector(LaserColors inColor)
{
   if(!MLD_PTR_CHECK(coordinateSystem)) return;
   AddCVector(coordinateSystem->AddCVector(coordinateSystem, inColor, FVector::ZeroVector, point.coordinate, CVectorMode::vector, "Position"));
}


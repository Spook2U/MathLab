// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "PlaneBase.h"

#include "CoordinateSystemBase.h"
#include "PointBase.h"
#include "CVectorBase.h"
#include "Lib/MathLabLibrary.h"



// Math Plane Structure ------------------------------------------------------------------------------------------------------------------------------

FMathPlane::FMathPlane() : isNormalSet(false), isDSet(false) {}
FMathPlane::FMathPlane(FVector inPosition, FVector inDirection1, FVector inDirection2) : position(inPosition), direction1(inDirection1), direction2(inDirection2), isNormalSet(false), isDSet(false)
{
   BuildNormal();
   BuildD();
}
FMathPlane::FMathPlane(FMathPoint inPoint, FVector inNormal) : normal(inNormal), isNormalSet(true), isDSet(false)
{
   BuildFromNormalform(inPoint, inNormal),
   BuildD();
}
FMathPlane::FMathPlane(float a, float b, float c, float d) : normal(FVector(a, b, c)), isNormalSet(true), isDSet(false)
{
   if(c != 0)      { BuildFromNormalform(FMathPoint({0, 0, -d/c}), normal); }
   else if(b != 0) { BuildFromNormalform(FMathPoint({0, -d/b, 0}), normal); }
   else            { BuildFromNormalform(FMathPoint({-d/a, 0, 0}), normal); }
   BuildD();
}

FMathPlane &FMathPlane::operator=(const FMathPlane &Other)
{
   this->position   = Other.position;
   this->direction1 = Other.direction1; 
   this->direction2 = Other.direction2;
   this->normal     = Other.normal;
   this->d          = Other.d;

   this->isNormalSet = Other.isNormalSet;
   this->isDSet = Other.isDSet;

   return *this;
}
bool FMathPlane::operator==(const FMathPlane &Other) const
{
   MathLabLibrary m;
   return ((m.MakeUnitVector(this->normal) == m.MakeUnitVector(Other.normal)) && m.IsPointInPlane(*this, FMathPoint(Other.position)));
}
bool FMathPlane::operator!=(const FMathPlane &Other) const
{
   return !(this == &Other);
}

void FMathPlane::BuildNormal()
{
   if(!isNormalSet)
   {
      normal = UKismetMathLibrary::Normal(UKismetMathLibrary::Cross_VectorVector(direction1, direction2));
      isNormalSet = true;
   }
}

void FMathPlane::BuildD()
{
   if(!isDSet)
   {
      d      = FVector::DotProduct(position, normal);
      isDSet = true;
   }
}

void FMathPlane::BuildFromNormalform(FMathPoint inPoint, FVector inNormal)
{
   if(inNormal.Z != 0)      
   { 
      position   = FVector(0, 0, UKismetMathLibrary::Dot_VectorVector(inPoint.coordinate, inNormal) / inNormal.Z);
      direction1 = FVector(1, 0, (-1) * inNormal.X / inNormal.Z);
      direction2 = FVector(0, 1, (-1) * inNormal.Y / inNormal.Z);
   }
   else if(inNormal.Y != 0) 
   { 
      position   = FVector(0, UKismetMathLibrary::Dot_VectorVector(inPoint.coordinate, inNormal) / inNormal.Y, 0);
      direction1 = FVector(1, (-1) * inNormal.X / inNormal.Y, 0);
      direction2 = FVector(0, (-1) * inNormal.Z / inNormal.Y, 1);
   }
   else            
   { 
      position   = FVector(UKismetMathLibrary::Dot_VectorVector(inPoint.coordinate, inNormal) / inNormal.X, 0, 0);
      direction1 = FVector((-1) * inNormal.Y / inNormal.X, 1, 0);
      direction2 = FVector((-1) * inNormal.Z / inNormal.X, 0, 1);
   }
}

FVector FMathPlane::GetNormal()
{
   if(!isNormalSet) { BuildNormal(); }
   return normal;
}

float FMathPlane::GetD()
{
   if(!isDSet) { BuildD(); }
   return d;
}

FString FMathPlane::ToString()
{
   return FString::Printf(TEXT("Position %s, Direction 1: %s, Direction 2: %s"), *position.ToString(), *direction1.ToString(), *direction2.ToString());
}
FString FMathPlane::ToStringShort()
{
   return FString::Printf(TEXT("(%s, %s, %s), (%s, %s, %s), (%s, %s, %s)"), *FString::SanitizeFloat(position.X),   *FString::SanitizeFloat(position.Y),   *FString::SanitizeFloat(position.Z), 
                                                                            *FString::SanitizeFloat(direction1.X), *FString::SanitizeFloat(direction1.Y), *FString::SanitizeFloat(direction1.Z),
                                                                            *FString::SanitizeFloat(direction2.X), *FString::SanitizeFloat(direction2.Y), *FString::SanitizeFloat(direction2.Z));
}



// Plane Class ---------------------------------------------------------------------------------------------------------------------------------------



APlaneBase::APlaneBase()
{
   plane = FMathPlane();
   planeMesh = nullptr;
}

// Unreal Events -------------------------------------------------------------------------------------------------------------------------------------

void APlaneBase::BeginPlay()
{
   Super::BeginPlay();
}

// Plane Setup ---------------------------------------------------------------------------------------------------------------------------------------

void APlaneBase::Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FMathPlane inPlane, PlaneMode inMode, FString inName)
{
   MLD_PTR_CHECK(inCoordinateSystem); if(!inCoordinateSystem) return;

   type = GeometryType::plane;
   plane = inPlane;
   plane.BuildNormal();
   plane.BuildD();
   mathDataString = inPlane.ToStringShort();

   Super::Init(inCoordinateSystem, inColor, inName);

   mode = inMode;

   switch(inMode)
   {
      case PlaneMode::plane: CreateCVector(inColor); break;
   }

   InitText(inName);
}

void APlaneBase::SetComponents(TArray<UStaticMeshComponent *> components, UTextRenderComponent *inText)
{
   for(UStaticMeshComponent *c : components)
   {
      MLD_PTR_CHECK(c); if(!c) continue;
      if(c->GetName().Equals("PlaneMesh")) { this->planeMesh = c; }
   }

   MLD_PTR_CHECK(planeMesh); if(!planeMesh) return;
   SetLaserMatTransparency(planeMesh, 0.1f);
   AddLaserComponent(planeMesh);

   if(!MLD_PTR_CHECK(inText)) return;
   nameRender = inText;
}

// Update Functions ----------------------------------------------------------------------------------------------------------------------------------

void APlaneBase::Update()
{
   Super::Update();
   Move(plane.position);
   BuildPlane();

}

void APlaneBase::BuildPlane()
{
   if(mode == PlaneMode::plane)
   {
      RotateLaserLookAt(plane.position, plane.position + plane.normal);
      SetLaserScale(planeMesh, FVector(coordinateSystem->MaxVisibleLength(), coordinateSystem->MaxVisibleLength(), NULL));
   }
}

// Setting Functions ---------------------------------------------------------------------------------------------------------------------------------

APlaneBase *APlaneBase::SetPlane(FMathPlane inPlane)
{
   plane = inPlane;
   mathDataString = inPlane.ToStringShort();

   constVectors[0]->SetCVector(FVector::ZeroVector, plane.position);
   constVectors[1]->SetCVector(plane.position, plane.direction1);
   constVectors[2]->SetCVector(plane.position, plane.direction2);
   constVectors[3]->SetCVector(plane.position, plane.normal);

   Update();

   return this;
}

FVector APlaneBase::GetNormal()
{
   return plane.GetNormal();
}

// Utility Functions----------------------------------------------------------------------------------------------------------------------------------

FString APlaneBase::ToString()
{
   return FString::Printf(TEXT("%s; %s"), *Super::ToString(), *plane.ToString());
}

// Constructing Vector Functions ---------------------------------------------------------------------------------------------------------------------

void APlaneBase::CreateCVector(LaserColors inColor)
{
   AddCVector(coordinateSystem->AddCVector(coordinateSystem, inColor, FVector::ZeroVector, plane.position, CVectorMode::vectorPoint, "Position"));
   AddCVector(coordinateSystem->AddCVector(coordinateSystem, inColor, plane.position, plane.direction1, CVectorMode::vectorPoint, "Direction 1"));
   AddCVector(coordinateSystem->AddCVector(coordinateSystem, inColor, plane.position, plane.direction2, CVectorMode::vectorPoint, "Direction 2"));
   AddCVector(coordinateSystem->AddCVector(coordinateSystem, inColor, plane.position, plane.normal, CVectorMode::vector, "Normal"));
}

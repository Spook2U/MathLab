// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "SphereBase.h"
#include "PointBase.h"
#include "LineBase.h"
#include "VectorStruct.h"

ASphereBase::ASphereBase()
{
   Radius = 0.f;
   Sphere = nullptr;
}

void ASphereBase::BeginPlay()
{
   Super::BeginPlay();
}



void ASphereBase::SetComponents(TArray<UStaticMeshComponent*> components)
{
   for(UStaticMeshComponent *c : components)
   {
      MLD_PTR_CHECK(c); if(!c) continue;
      if(c->GetName().Equals("Sphere")) { this->Sphere = c; }
   }

   MLD_PTR_CHECK(Sphere); if(!Sphere) return;
   SetLaserMatTransparency(Sphere, 0.1f);
   AddLaserComponent(Sphere);
}



void ASphereBase::SetValuesSphere(ACoordinateSystemBase* coordinateSystem, LaserColors color, FVector coordinate, float radius)
{
   MLD_PTR_CHECK(coordinateSystem); if(!coordinateSystem) return;

   this->Radius = radius;
   InitPoint(coordinateSystem, color, coordinate);
}



void ASphereBase::Update()
{
   Super::Update();
   BuildSphere();
}



void ASphereBase::BuildSphere()
{
   ScaleSphere(Sphere, Radius);
}

// Protected ----------------------------------------------------------------------------------------

void ASphereBase::CreateVectorGuides(LaserColors color)
{
   Super::CreateVectorGuides(color);
   AVectorStruct *pointVectorStruct = nullptr;

   for(AVectorStruct *v : VectorGuides)
   {
      MLD_PTR_CHECK(v); if(!v) continue;
      if(v->A.Equals(FVector::ZeroVector) && v->B.Equals(Coordinate))
      {
         pointVectorStruct = v;
         break;
      } 
   }
   pointVectorStruct->SetVisibilityPointB(true);

   AddVectorGuide(CoordinateSystem->AddVectorStruct(color, Coordinate, Coordinate + FVector(Radius, 0, 0), VectorStructMode::segment));
}



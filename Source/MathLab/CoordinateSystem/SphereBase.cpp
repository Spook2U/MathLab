// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "SphereBase.h"
#include "PointBase.h"
#include "LineBase.h"
#include "VectorStruct.h"

ASphereBase::ASphereBase()
{
   sphere = FMathSphere();
   sphereMesh = nullptr;
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
      if(c->GetName().Equals("SphereMesh")) { this->sphereMesh = c; }
   }

   MLD_PTR_CHECK(sphereMesh); if(!sphereMesh) return;
   SetLaserMatTransparency(sphereMesh, 0.1f);
   AddLaserComponent(sphereMesh);
}



void ASphereBase::InitSphere(ACoordinateSystemBase* coordinateSystem, LaserColors color, FMathSphere inSphere)
{
   MLD_PTR_CHECK(coordinateSystem); if(!coordinateSystem) return;

   this->sphere = inSphere;
   InitPoint(coordinateSystem, color, sphere);
   this->type = GeometryType::sphere; //after InitPoint() to override the value from point
}



void ASphereBase::Update()
{
   Super::Update();
   BuildSphere();
}



void ASphereBase::BuildSphere()
{
   ScaleSphere(sphereMesh, sphere.Radius);
}

// Protected ----------------------------------------------------------------------------------------

void ASphereBase::CreateVectorGuides(LaserColors color)
{
   Super::CreateVectorGuides(color);
   AVectorStruct *pointVectorStruct = nullptr;

   for(AVectorStruct *v : VectorGuides)
   {
      MLD_PTR_CHECK(v); if(!v) continue;
      if(v->A.Equals(FVector::ZeroVector) && v->B.Equals(point.Coordinate))
      {
         pointVectorStruct = v;
         break;
      } 
   }
   pointVectorStruct->SetVisibilityPointB(true);

   AddVectorGuide(CoordinateSystem->AddVectorStruct(color, sphere.Coordinate, sphere.Coordinate + FVector(sphere.Radius, 0, 0), VectorStructMode::segment));
}



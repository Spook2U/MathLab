// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "SphereBase.h"

#include "CoordinateSystemBase.h"
#include "VectorStruct.h"



ASphereBase::ASphereBase()
{
   sphere = FMathSphere();
   sphereMesh = nullptr;
}

FString FMathSphere::ToString()
{
   return FString::Printf(TEXT("%s, Radius:s %f"), *Super::ToString(), radius);
}

// -------------------------------------------------------------------------------------------------

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
   AddLaserComponent(sphereMesh);
}



void ASphereBase::InitSphere(ACoordinateSystemBase *inCoordinateSystem, LaserColors color, FMathSphere inSphere)
{
   MLD_PTR_CHECK(inCoordinateSystem); if(!inCoordinateSystem) return;

   this->sphere = inSphere;
   InitPoint(inCoordinateSystem, color, sphere);
   this->type = GeometryType::sphere; //after InitPoint() to override the value from point
}



void ASphereBase::Update()
{
   Super::Update();
   BuildSphere();
}



void ASphereBase::BuildSphere()
{
   ScaleSphere(sphereMesh, sphere.radius);
}

FString ASphereBase::ToString()
{
   return FString::Printf(TEXT("%s, Radius: %f"), *Super::ToString(), sphere.radius);
}

// Protected ----------------------------------------------------------------------------------------

void ASphereBase::CreateVectorGuides(LaserColors color)
{
   Super::CreateVectorGuides(color);
   AVectorStruct *pointVectorStruct = nullptr;

   for(AVectorStruct *v : vectorGuides)
   {
      MLD_PTR_CHECK(v); if(!v) continue;
      if(v->a.Equals(FVector::ZeroVector) && v->b.Equals(point.coordinate))
      {
         pointVectorStruct = v;
         break;
      } 
   }
   pointVectorStruct->SetVisibilityPointB(true);

   AddVectorGuide(coordinateSystem->AddVectorStruct(color, sphere.coordinate, sphere.coordinate + FVector(sphere.radius, 0, 0), VectorStructMode::segment));
}


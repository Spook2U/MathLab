// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "SphereBase.h"

#include "CoordinateSystemBase.h"
#include "CVectorBase.h"



FMathSphere::FMathSphere() {}
FMathSphere::FMathSphere(FVector inCenter, float inRadius) : center(inCenter), radius(inRadius) {}

FString FMathSphere::ToString()
{
   return FString::Printf(TEXT("%s, Radius:s %f"), *center.ToString(), radius);
}
FString FMathSphere::ToStringShort()
{
   return FString::Printf(TEXT("(%s, %s, %s), R:%s)"), *FString::SanitizeFloat(center.X),  *FString::SanitizeFloat(center.Y),  *FString::SanitizeFloat(center.Z), *FString::SanitizeFloat(radius));
}

// -------------------------------------------------------------------------------------------------

ASphereBase::ASphereBase()
{
   sphere = FMathSphere();
   sphereMesh = nullptr;
}



void ASphereBase::BeginPlay()
{
   Super::BeginPlay();
}



void ASphereBase::SetComponents(TArray<UStaticMeshComponent*> components, UTextRenderComponent *inText)
{
   for(UStaticMeshComponent *c : components)
   {
      MLD_PTR_CHECK(c); if(!c) continue;
      if(c->GetName().Equals("SphereMesh")) { this->sphereMesh = c; }
   }

   MLD_PTR_CHECK(sphereMesh); if(!sphereMesh) return;
   AddLaserComponent(sphereMesh);

   if(!MLD_PTR_CHECK(inText)) return;
   nameText = inText;
}



void ASphereBase::Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FMathSphere inSphere, FString inName)
{
   MLD_PTR_CHECK(inCoordinateSystem); if(!inCoordinateSystem) return;

   type = GeometryType::sphere;
   sphere = inSphere;
   mathDataString = inSphere.ToStringShort();
   Super::Init(inCoordinateSystem, inColor, inName);
}



void ASphereBase::Update()
{
   Super::Update();
   SetPosition(sphere.center);
   BuildSphere();
}



void ASphereBase::BuildSphere()
{
   ScaleSphere(sphereMesh, sphere.radius);
}

FString ASphereBase::ToString()
{
   return FString::Printf(TEXT("%s; %s"), *Super::ToString(), *sphere.ToString());
}

// Protected ----------------------------------------------------------------------------------------

void ASphereBase::CreateCVector(LaserColors inColor)
{
   AddCVector(coordinateSystem->AddCVector(inColor, FVector::ZeroVector, sphere.center, CVectorMode::vectorPoint));
   AddCVector(coordinateSystem->AddCVector(inColor, sphere.center, sphere.center + FVector(sphere.radius, 0, 0), CVectorMode::segment));
}


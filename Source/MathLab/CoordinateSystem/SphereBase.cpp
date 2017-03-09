// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "SphereBase.h"

#include "CoordinateSystemBase.h"
#include "CVectorBase.h"



// Math Sphere Structure -----------------------------------------------------------------------------------------------------------------------------

FMathSphere::FMathSphere() {}
FMathSphere::FMathSphere(FVector inCenter, float inRadius) : center(inCenter), radius(inRadius) {}

FMathSphere &FMathSphere::operator=(const FMathSphere &Other)
{
   this->center = Other.center;
   this->radius = Other.radius;

   return *this;
}
bool FMathSphere::operator==(const FMathSphere &Other) const
{
   return (this->center == Other.center) && (this->radius == Other.radius);
}
bool FMathSphere::operator!=(const FMathSphere &Other) const
{
   return !(this == &Other);
}

FString FMathSphere::ToString()
{
   return FString::Printf(TEXT("%s, Radius:s %f"), *center.ToString(), radius);
}
FString FMathSphere::ToStringShort()
{
   return FString::Printf(TEXT("(%s, %s, %s), R:%s)"), *FString::SanitizeFloat(center.X),  *FString::SanitizeFloat(center.Y),  *FString::SanitizeFloat(center.Z), *FString::SanitizeFloat(radius));
}



// Sphere Class --------------------------------------------------------------------------------------------------------------------------------------



ASphereBase::ASphereBase()
{
   sphere = FMathSphere();
   sphereMesh = nullptr;
}

// Unreal Events -------------------------------------------------------------------------------------------------------------------------------------

void ASphereBase::BeginPlay()
{
   Super::BeginPlay();
}

// Sphere Setup --------------------------------------------------------------------------------------------------------------------------------------

void ASphereBase::Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FMathSphere inSphere, FName inName)
{
   MLD_PTR_CHECK(inCoordinateSystem); if(!inCoordinateSystem) return;

   type = GeometryType::sphere;
   sphere = inSphere;
   nameMathData = FName(*inSphere.ToStringShort());
   Super::Init(inCoordinateSystem, inColor, inName);
   CreateCVector(inColor);
   InitName(inName);
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
   nameRender = inText;
}

// Update Functions ----------------------------------------------------------------------------------------------------------------------------------

void ASphereBase::Update()
{
   Super::Update();
   Move(sphere.center);
   BuildSphere();
}

void ASphereBase::BuildSphere()
{
   ScaleSphere(sphereMesh, sphere.radius);
}

// Setting Functions ---------------------------------------------------------------------------------------------------------------------------------

ASphereBase *ASphereBase::SetSphere(FMathSphere inSphere)
{
   sphere = inSphere;
   nameMathData = FName(*inSphere.ToStringShort());

   constVectors[0]->SetCVector(FVector::ZeroVector, sphere.center);
   constVectors[0]->SetCVector(sphere.center, sphere.center + FVector(sphere.radius, 0, 0));

   Update();

   return this;
}

// Utility Functions----------------------------------------------------------------------------------------------------------------------------------

FString ASphereBase::ToString()
{
   return FString::Printf(TEXT("%s; %s"), *Super::ToString(), *sphere.ToString());
}

// Constructing Vector Functions ---------------------------------------------------------------------------------------------------------------------

void ASphereBase::CreateCVector(LaserColors inColor)
{
   AddCVector(coordinateSystem->AddCVector(coordinateSystem, inColor, FVector::ZeroVector, sphere.center, CVectorMode::vectorPoint, "Position"));
   AddCVector(coordinateSystem->AddCVector(coordinateSystem, inColor, sphere.center, sphere.center + FVector(sphere.radius, 0, 0), CVectorMode::segment, FName(*FString::Printf(TEXT("Radius: %s"), *FString::SanitizeFloat(sphere.radius)))));
}


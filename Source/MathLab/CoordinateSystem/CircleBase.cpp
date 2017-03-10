// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "CircleBase.h"

#include "PlaneBase.h"
#include "Lib/MathLabLibrary.h"



// Math Circle Structure -----------------------------------------------------------------------------------------------------------------------------

FMathCircle::FMathCircle() {}
FMathCircle::FMathCircle(FVector inCenter, FVector inNormal, float inRadius)   : center(inCenter), normal(inNormal), radius(inRadius) {}

FMathCircle &FMathCircle::operator=(const FMathCircle &Other)
{
   this->center = Other.center;
   this->normal = Other.normal; 
   this->radius = Other.radius;

   return *this;
}
bool FMathCircle::operator==(const FMathCircle &Other) const
{
   MathLabLibrary m;
   return (this->center == Other.center) && (m.MakeUnitVector(this->normal) == m.MakeUnitVector(Other.normal)) && (this->radius == Other.radius);
}
bool FMathCircle::operator!=(const FMathCircle &Other) const
{
   return !(this == &Other);
}

FString FMathCircle::ToString()
{
   return FString::Printf(TEXT("Center: %s, Radius: %f; laying on plane with Normal: %s"), *center.ToString(), radius, *normal.ToString());
}
FString FMathCircle::ToStringShort()
{
   return FString::Printf(TEXT("(%s, %s, %s), R:%s)"), *FString::SanitizeFloat(center.X),  *FString::SanitizeFloat(center.Y),  *FString::SanitizeFloat(center.Z), *FString::SanitizeFloat(radius));
}



// Circle Class --------------------------------------------------------------------------------------------------------------------------------------



// Circle Setup --------------------------------------------------------------------------------------------------------------------------------------

void ACircleBase::Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FMathCircle inCircle, FName inName)
{
   if(!MLD_PTR_CHECK(inCoordinateSystem)) return;

   type = GeometryType::circle;
   circle = inCircle;
   nameMathData = FName(*inCircle.ToStringShort());

   Super::Init(inCoordinateSystem, inColor, inName);
   InitName(inName);
}

void ACircleBase::SetComponents(TArray<UStaticMeshComponent*> components, UTextRenderComponent *inText)
{
   for(UStaticMeshComponent *c : components)
   {
      if(!MLD_PTR_CHECK(c)) continue;
      if(c->GetName().Equals("CircleMesh"))       { circleMesh = c; }
      if(c->GetName().Equals("CircleMeshBorder")) { circleMeshBorder = c; }
   }

   if(!(MLD_PTR_CHECK(circleMesh) && MLD_PTR_CHECK(circleMeshBorder) && MLD_PTR_CHECK(inText))) return;
   AddLaserComponent(circleMesh);
   AddLaserComponent(circleMeshBorder);

   nameRender = inText;
}

// Update Functions ----------------------------------------------------------------------------------------------------------------------------------

void ACircleBase::Update()
{
   Super::Update();
   Move(circle.center);
   BuildCircle();
}

void ACircleBase::BuildCircle()
{
   RotateLaserLookAt(circle.center, circle.center + circle.normal);
   ScalePlane(circleMesh, circle.radius*1.085);
   ScalePlane(circleMeshBorder, circle.radius*1.085);

   float sizeFactor = 1;

   if     (circle.radius <  1) sizeFactor = 0.0555;
   else if(circle.radius <  2) sizeFactor = 0.0455;
   else if(circle.radius <  3) sizeFactor = 0.0425;
   else if(circle.radius <  4) sizeFactor = 0.0406;
   else if(circle.radius <  5) sizeFactor = 0.0401;
   else if(circle.radius <  6) sizeFactor = 0.0399;
   else if(circle.radius <  7) sizeFactor = 0.0397;
   else if(circle.radius <  8) sizeFactor = 0.0395;
   else if(circle.radius <  9) sizeFactor = 0.0394;
   else if(circle.radius < 10) sizeFactor = 0.0393;
   else if(circle.radius < 11) sizeFactor = 0.0392;
   else                        sizeFactor = 0.0391;

   if(!MLD_PTR_CHECK(circleMeshBorder)) return;
   circleMeshBorder->SetScalarParameterValueOnMaterials(FName(TEXT("Substraction Radius")), 0.5 - sizeFactor);
}

// Setting Functions ---------------------------------------------------------------------------------------------------------------------------------

ACircleBase *ACircleBase::SetCircle(FMathCircle inCircle)
{
   circle = inCircle;
   nameMathData = FName(*inCircle.ToStringShort());

   //SetCvector
   //constVectors[0]->SetCVector(...);

   Update();

   return this;
}

// Utility Functions----------------------------------------------------------------------------------------------------------------------------------

FString ACircleBase::ToString()
{
   return FString::Printf(TEXT("%s; %s"), *Super::ToString(), *circle.ToString());
}

// Constructing Vector Functions ---------------------------------------------------------------------------------------------------------------------

void ACircleBase::CreateCVector(LaserColors inColor)
{}

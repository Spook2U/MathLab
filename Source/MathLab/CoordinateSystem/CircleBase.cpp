// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "CircleBase.h"



FMathCircle::FMathCircle() {}
FMathCircle::FMathCircle(FVector inCenter, FVector inNormal, float inRadius)
{
   Center = inCenter;
   Normal = inNormal;
   Radius = inRadius;
}

void ACircleBase::SetComponents(TArray<UStaticMeshComponent*> components)
{
   for(UStaticMeshComponent *c : components)
   {
      MLD_PTR_CHECK(c); if(!c) continue;
      if(c->GetName().Equals("CircleMesh"))       { CircleMesh = c; }
      if(c->GetName().Equals("CircleMeshBorder")) { CircleMeshBorder = c; }
   }

   MLD_PTR_CHECK(CircleMesh);
   MLD_PTR_CHECK(CircleMeshBorder);
   if(!(CircleMesh && CircleMeshBorder)) return;
   AddLaserComponent(CircleMesh);
   AddLaserComponent(CircleMeshBorder);
}

void ACircleBase::InitCircle(ACoordinateSystemBase *coordinateSystem, LaserColors color, FMathCircle inCircle)
{
   MLD_PTR_CHECK(coordinateSystem); if(!coordinateSystem) return;

   SetValuesGeometry(coordinateSystem, color);
   circle = inCircle;

   type = GeometryType::circle;
}

void ACircleBase::Update()
{
   Super::Update();
   SetPosition(circle.Center);
   BuildCircle();
}

void ACircleBase::BuildCircle()
{
   RotateLaserLookAt(circle.Center, circle.Center + circle.Normal);
   ScalePlane(CircleMesh, circle.Radius*1.085);
   ScalePlane(CircleMeshBorder, circle.Radius*1.085);

   float sizeFactor = 1;

   if     (circle.Radius <  1) sizeFactor = 0.0555;
   else if(circle.Radius <  2) sizeFactor = 0.0455;
   else if(circle.Radius <  3) sizeFactor = 0.0425;
   else if(circle.Radius <  4) sizeFactor = 0.0406;
   else if(circle.Radius <  5) sizeFactor = 0.0401;
   else if(circle.Radius <  6) sizeFactor = 0.0399;
   else if(circle.Radius <  7) sizeFactor = 0.0397;
   else if(circle.Radius <  8) sizeFactor = 0.0395;
   else if(circle.Radius <  9) sizeFactor = 0.0394;
   else if(circle.Radius < 10) sizeFactor = 0.0393;
   else if(circle.Radius < 11) sizeFactor = 0.0392;
   else                        sizeFactor = 0.0391;

   CircleMeshBorder->SetScalarParameterValueOnMaterials(FName(TEXT("Substraction Radius")), 0.5 - sizeFactor);
}

// Protected ----------------------------------------------------------------------------------------

void ACircleBase::CreateVectorGuides(LaserColors color)
{
}

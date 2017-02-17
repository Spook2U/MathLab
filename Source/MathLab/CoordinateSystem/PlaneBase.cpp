// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "PlaneBase.h"

#include "CoordinateSystemBase.h"
#include "PointBase.h"
#include "VectorStruct.h"



FMathPlane::FMathPlane() {}
FMathPlane::FMathPlane(FVector inPosition, FVector inDirection1, FVector inDirection2)
{
   position = inPosition;
   direction1 = inDirection1;
   direction2 = inDirection2;
   normal = UKismetMathLibrary::Normal(UKismetMathLibrary::Cross_VectorVector(inDirection1, inDirection2));
}
FMathPlane::FMathPlane(FMathPoint inPoint, FVector inNormal)
{
   position   = FVector(0, 0, UKismetMathLibrary::Dot_VectorVector(inPoint.coordinate, inNormal) / inNormal.Z);
   direction1 = FVector(1, 0, (-1) * inNormal.X / inNormal.Z);
   direction2 = FVector(0, 1, (-1) * inNormal.Y / inNormal.Z);
   normal = inNormal;
}



APlaneBase::APlaneBase()
{
   plane = FMathPlane();
   planeMesh = nullptr;
}

void APlaneBase::BeginPlay()
{
   Super::BeginPlay();
}



void APlaneBase::SetComponents(TArray<UStaticMeshComponent *> components)
{
   for(UStaticMeshComponent *c : components)
   {
      MLD_PTR_CHECK(c); if(!c) continue;
      if(c->GetName().Equals("PlaneMesh")) { this->planeMesh = c; }
   }

   MLD_PTR_CHECK(planeMesh); if(!planeMesh) return;
   SetLaserMatTransparency(planeMesh, 0.1f);
   AddLaserComponent(planeMesh);
}



void APlaneBase::InitPlane(ACoordinateSystemBase *inCoordinateSystem, LaserColors color, FMathPlane inPlane, PlaneMode inMode)
{
   MLD_PTR_CHECK(inCoordinateSystem); if(!inCoordinateSystem) return;

   SetValuesGeometry(inCoordinateSystem, color);
   plane = inPlane;
   plane.normal = UKismetMathLibrary::Normal(UKismetMathLibrary::Cross_VectorVector(plane.direction1, plane.direction2));

   mode = inMode;
   type = GeometryType::plane;

   switch(inMode)
   {
      case PlaneMode::plane: CreateVectorGuides(color); break;
   }
}

FVector APlaneBase::GetNormal()
{
   return plane.normal;
}



void APlaneBase::Update()
{
   Super::Update();
   SetPosition(plane.position);
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

// Protected ----------------------------------------------------------------------------------------

void APlaneBase::CreateVectorGuides(LaserColors color)
{
   AddVectorGuide(coordinateSystem->AddVectorStruct(color, FVector::ZeroVector, plane.position, VectorStructMode::vectorPoint));
   AddVectorGuide(coordinateSystem->AddVectorStruct(color, plane.position, plane.direction1, VectorStructMode::vectorPoint));
   AddVectorGuide(coordinateSystem->AddVectorStruct(color, plane.position, plane.direction2, VectorStructMode::vectorPoint));
   AddVectorGuide(coordinateSystem->AddVectorStruct(color, plane.position, plane.normal, VectorStructMode::vector));
}

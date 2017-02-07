// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "PlaneBase.h"
#include "PointBase.h"
#include "LineBase.h"

APlaneBase::APlaneBase()
{
   plane = FMathPlane();
   PlaneMesh = nullptr;
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
      if(c->GetName().Equals("PlaneMesh")) { this->PlaneMesh = c; }
   }

   MLD_PTR_CHECK(PlaneMesh); if(!PlaneMesh) return;
   SetLaserMatTransparency(PlaneMesh, 0.1f);
   AddLaserComponent(PlaneMesh);
}



void APlaneBase::InitPlane(ACoordinateSystemBase *coordinateSystem, LaserColors color, FMathPlane inPlane, PlaneMode mode)
{
   MLD_PTR_CHECK(coordinateSystem); if(!coordinateSystem) return;

   SetValuesGeometry(coordinateSystem, color);
   this->plane = inPlane;
   plane.Normal = UKismetMathLibrary::Normal(UKismetMathLibrary::Cross_VectorVector(plane.Direction1, plane.Direction2));

   this->type = GeometryType::plane;

   switch(mode)
   {
      case PlaneMode::plane: CreateVectorGuides(color); break;
   }
}

FVector APlaneBase::GetNormal()
{
   return plane.Normal;
}



void APlaneBase::Update()
{
   Super::Update();
   SetPosition(plane.Position);
   BuildPlane();
}



void APlaneBase::BuildPlane()
{
   if(Mode == PlaneMode::plane)
   {
      RotateLaserLookAt(plane.Position, plane.Position + plane.Normal);
      SetLaserScale(PlaneMesh, FVector(CoordinateSystem->MaxVisibleLength(), CoordinateSystem->MaxVisibleLength(), NULL));
   }
}

// Protected ----------------------------------------------------------------------------------------

void APlaneBase::CreateVectorGuides(LaserColors color)
{
   AddVectorGuide(CoordinateSystem->AddVectorStruct(color, FVector::ZeroVector, plane.Position, VectorStructMode::vectorPoint));
   AddVectorGuide(CoordinateSystem->AddVectorStruct(color, plane.Position, plane.Direction1, VectorStructMode::vectorPoint));
   AddVectorGuide(CoordinateSystem->AddVectorStruct(color, plane.Position, plane.Direction2, VectorStructMode::vectorPoint));
   AddVectorGuide(CoordinateSystem->AddVectorStruct(color, plane.Position, plane.Normal, VectorStructMode::vector));
}

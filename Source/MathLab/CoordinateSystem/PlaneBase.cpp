// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "PlaneBase.h"
#include "PointBase.h"
#include "LineBase.h"

APlaneBase::APlaneBase()
{
   Position   = FVector::ZeroVector;
   Direction1 = FVector::ZeroVector;
   Direction2 = FVector::ZeroVector;
   Normal     = FVector::ZeroVector;
   Plane = nullptr;
}

void APlaneBase::BeginPlay()
{
   Super::BeginPlay();
}



void APlaneBase::SetComponents(TArray<UStaticMeshComponent *> components)
{
   for(UStaticMeshComponent *c : components)
   {
      MLD_PTR_CHECK(c);
      if(c) 
      { 
         if(c->GetName().Equals("Plane")) { this->Plane = c; }
      }
   }

   MLD_PTR_CHECK(Plane); if(!Plane) return;
   SetLaserMatTransparency(Plane, 0.1f);
   AddLaserComponent(Plane);
}



void APlaneBase::SetValuesPlane(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector position, FVector direction1, FVector direction2, PlaneMode mode)
{
   SetValuesGeometry(coordinateSystem, color);
   this->Position = position;
   this->Direction1 = direction1;
   this->Direction2 = direction2;

   this->Normal = UKismetMathLibrary::Normal(UKismetMathLibrary::Cross_VectorVector(Direction1, Direction2));

   switch(mode)
   {
      case PlaneMode::plane: CreateVectorGuides(color); break;
   }

}



void APlaneBase::Update()
{
   Super::Update();
   SetPosition(Position);
   BuildPlane();
}



void APlaneBase::BuildPlane()
{
   MLD_PTR_CHECK(Plane); if(!Plane) return;
   
   if(Mode == PlaneMode::plane)
   {
      RotateLaserLookAt(Position, Position + Normal);
      SetLaserScale(Plane, FVector(CoordinateSystem->MaxVisibleLength(), CoordinateSystem->MaxVisibleLength(), NULL));
   }
}

// Protected ----------------------------------------------------------------------------------------

void APlaneBase::CreateVectorGuides(LaserColors color)
{
   MLD_PTR_CHECK(CoordinateSystem); if(!CoordinateSystem) return;

   AddVectorGuide(CoordinateSystem->AddVectorStruct(color, FVector::ZeroVector, Position, VectorStructMode::vectorPoint));
   AddVectorGuide(CoordinateSystem->AddVectorStruct(color, Position, Direction1, VectorStructMode::vectorPoint));
   AddVectorGuide(CoordinateSystem->AddVectorStruct(color, Position, Direction2, VectorStructMode::vectorPoint));
   AddVectorGuide(CoordinateSystem->AddVectorStruct(color, Position, Normal, VectorStructMode::vector));
}

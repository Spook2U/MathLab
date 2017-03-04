// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "PlaneBase.h"

#include "CoordinateSystemBase.h"
#include "PointBase.h"
#include "CVector.h"



FMathPlane::FMathPlane() {}
FMathPlane::FMathPlane(FVector inPosition, FVector inDirection1, FVector inDirection2) : position(inPosition), direction1(inDirection1), direction2(inDirection2)
{
   normal = UKismetMathLibrary::Normal(UKismetMathLibrary::Cross_VectorVector(inDirection1, inDirection2));
}
FMathPlane::FMathPlane(FMathPoint inPoint, FVector inNormal) : normal(inNormal)
{
   position   = FVector(0, 0, UKismetMathLibrary::Dot_VectorVector(inPoint.coordinate, inNormal) / inNormal.Z);
   direction1 = FVector(1, 0, (-1) * inNormal.X / inNormal.Z);
   direction2 = FVector(0, 1, (-1) * inNormal.Y / inNormal.Z);
}

FString FMathPlane::ToString()
{
   return FString::Printf(TEXT("Position %s, Direction 1: %s, Direction 2: %s"), *position.ToString(), *direction1.ToString(), *direction2.ToString());
}
FString FMathPlane::ToStringShort()
{
   return FString::Printf(TEXT("(%s, %s, %s), (%s, %s, %s), (%s, %s, %s)"), *FString::SanitizeFloat(position.X),   *FString::SanitizeFloat(position.Y),   *FString::SanitizeFloat(position.Z), 
                                                                            *FString::SanitizeFloat(direction1.X), *FString::SanitizeFloat(direction1.Y), *FString::SanitizeFloat(direction1.Z),
                                                                            *FString::SanitizeFloat(direction2.X), *FString::SanitizeFloat(direction2.Y), *FString::SanitizeFloat(direction2.Z));
}

// -------------------------------------------------------------------------------------------------

APlaneBase::APlaneBase()
{
   plane = FMathPlane();
   planeMesh = nullptr;
}

void APlaneBase::BeginPlay()
{
   Super::BeginPlay();
}



void APlaneBase::SetComponents(TArray<UStaticMeshComponent *> components, UTextRenderComponent *inText)
{
   for(UStaticMeshComponent *c : components)
   {
      MLD_PTR_CHECK(c); if(!c) continue;
      if(c->GetName().Equals("PlaneMesh")) { this->planeMesh = c; }
   }

   MLD_PTR_CHECK(planeMesh); if(!planeMesh) return;
   SetLaserMatTransparency(planeMesh, 0.1f);
   AddLaserComponent(planeMesh);

   if(!MLD_PTR_CHECK(inText)) return;
   nameText = inText;
}



void APlaneBase::Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FMathPlane inPlane, PlaneMode inMode, FString inName)
{
   MLD_PTR_CHECK(inCoordinateSystem); if(!inCoordinateSystem) return;

   type = GeometryType::plane;
   plane = inPlane;
   mathDataString = inPlane.ToStringShort();
   plane.normal = UKismetMathLibrary::Normal(UKismetMathLibrary::Cross_VectorVector(plane.direction1, plane.direction2));

   Super::Init(inCoordinateSystem, inColor, inName);
   
   mode = inMode;

   switch(inMode)
   {
      case PlaneMode::plane: CreateCVector(inColor); break;
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

FString APlaneBase::ToString()
{
   return FString::Printf(TEXT("%s; %s"), *Super::ToString(), *plane.ToString());
}

// Protected ----------------------------------------------------------------------------------------

void APlaneBase::CreateCVector(LaserColors inColor)
{
   AddCVector(coordinateSystem->AddCVector(inColor, FVector::ZeroVector, plane.position, CVectorMode::vectorPoint));
   AddCVector(coordinateSystem->AddCVector(inColor, plane.position, plane.direction1, CVectorMode::vectorPoint));
   AddCVector(coordinateSystem->AddCVector(inColor, plane.position, plane.direction2, CVectorMode::vectorPoint));
   AddCVector(coordinateSystem->AddCVector(inColor, plane.position, plane.normal, CVectorMode::vector));
}

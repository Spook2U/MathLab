// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "PlaneBase.h"
#include "PointBase.h"
#include "LineBase.h"

APlaneBase::APlaneBase()
{
   Position = FVector();
   Direction1 = FVector();
   Direction2 = FVector();
   Normal = FVector();
}

// Unreal Events -----------------------------------------------------------------------------------

void APlaneBase::BeginPlay()
{
   Super::BeginPlay();
}

// Initialise --------------------------------------------------------------------------------------

void APlaneBase::SetComponents(TArray<UStaticMeshComponent *> components)
{
   for(UStaticMeshComponent *c : components)
   {
      if(c->GetName().Equals("Plane")) { this->Plane = c; }
   }

   SetLaserMatTransparency(Plane, 0.1f);
   AddLaserComponent(Plane);
}

// Update -------------------------------------------------------------------------------------------

void APlaneBase::Update()
{
   Super::Update();
   SetPosition(Position);
   BuildPlane();
}

// Setup --------------------------------------------------------------------------------------------

void APlaneBase::BuildPlane()
{
   SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CoordinateToLocation(Position), CoordinateToLocation(Position + Normal)));
   
   Plane->SetWorldScale3D(FVector(CoordinateSystem->MaxVisibleLength(), CoordinateSystem->MaxVisibleLength(), Plane->GetComponentScale().Z));
}

// -------------------------------------------------------------------------------------------------

void APlaneBase::SetValuesPlane(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector position, FVector direction1, FVector direction2, PlaneMode mode)
{
   SetValues(coordinateSystem, color);
   this->Position = position;
   this->Direction1 = direction1;
   this->Direction2 = direction2;

   this->Normal = UKismetMathLibrary::Normal(UKismetMathLibrary::Cross_VectorVector(Direction1, Direction2));

   switch(mode)
   {
      case PlaneMode::plane: CreateGuides(color); break;
   }

}

// Protected ----------------------------------------------------------------------------------------

void APlaneBase::CreateGuides(LaserColors color)
{
   if(IsGuide) return;

   AddGuide(CoordinateSystem->AddPoint(color, true, Position));
   AddGuide(CoordinateSystem->AddPoint(color, true, Position + Direction1));
   AddGuide(CoordinateSystem->AddPoint(color, true, Position + Direction2));

   AddGuide(CoordinateSystem->AddLine(color, true, FVector::ZeroVector, Position,  LineMode::vector));
   AddGuide(CoordinateSystem->AddLine(color, true, Position, Direction1, LineMode::vector));
   AddGuide(CoordinateSystem->AddLine(color, true, Position, Direction2, LineMode::vector));
   AddGuide(CoordinateSystem->AddLine(color, true, Position, Normal,     LineMode::vector));
}

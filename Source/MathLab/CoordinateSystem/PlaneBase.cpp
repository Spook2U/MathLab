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

void APlaneBase::SetComponents(UStaticMeshComponent *plane, TArray<UStaticMeshComponent *> laserComponents)
{
   this->Plane = plane;
   this->Plane->SetScalarParameterValueOnMaterials("Transparency", 0.1f);

   for(UStaticMeshComponent *l : laserComponents)
   {
      LaserCompoents.Add(l);
   }
}

// Update -------------------------------------------------------------------------------------------

void APlaneBase::Update()
{
   Super::Update();
   SetPosition(Position);
   BuildPlane();
}

// -------------------------------------------------------------------------------------------------

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
      case PlaneMode::plane: CreateGuidesPlane(color); break;
   }

}

// Protected ----------------------------------------------------------------------------------------

void APlaneBase::CreateGuidesPlane(LaserColors color)
{
   if(IsGuide) return;

   AddGuide(CoordinateSystem->AddPoint(color, true, Position));
   AddGuide(CoordinateSystem->AddPoint(color, true, Position + Direction1));
   AddGuide(CoordinateSystem->AddPoint(color, true, Position + Direction2));

   AddGuide(CoordinateSystem->AddLine(color, true, FVector(), Position,  LineMode::vector));
   AddGuide(CoordinateSystem->AddLine(color, true, Position, Direction1, LineMode::vector));
   AddGuide(CoordinateSystem->AddLine(color, true, Position, Direction2, LineMode::vector));
   AddGuide(CoordinateSystem->AddLine(color, true, Position, Normal,     LineMode::vector));
}

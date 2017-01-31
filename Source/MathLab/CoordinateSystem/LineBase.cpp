// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "LineBase.h"

ALineBase::ALineBase()
{
   Position  = FVector::ZeroVector;
   Direction = FVector::ZeroVector;
   Line = nullptr;
   Arrowhead = nullptr;
}

void ALineBase::BeginPlay()
{
   Super::BeginPlay();
}




void ALineBase::SetComponents(TArray<UStaticMeshComponent*> components)
{
   for(UStaticMeshComponent *c : components)
   {
      MLD_PTR_CHECK(c); if(!c) continue;
      if(c->GetName().Equals("Line"))      { this->Line      = c; }
      if(c->GetName().Equals("Arrowhead")) { this->Arrowhead = c; }
   }

   MLD_PTR_CHECK(Line);
   MLD_PTR_CHECK(Arrowhead);
   if(!(Line && Arrowhead)) return;

   InitScaleLine(Line);
   InitScaleArrowhead(Arrowhead);
   AddLaserComponent(Line);
   AddLaserComponent(Arrowhead);

   Arrowhead->SetVisibility(false);
}



void ALineBase::SetValuesLine(ACoordinateSystemBase * coordinateSystem, LaserColors color, FVector position, FVector direction, LineMode mode)
{
   MLD_PTR_CHECK(coordinateSystem); if(!coordinateSystem) return;

   SetValuesGeometry(coordinateSystem, color);
   this->Position = position;
   this->Direction = direction;
   this->Mode = mode;

   switch(Mode)
   {
      case LineMode::line:    CreateVectorGuides(color); break;
      case LineMode::segment: break;
      case LineMode::vector:  Arrowhead->SetVisibility(true); break;
   }

}



void ALineBase::Update()
{
   Super::Update();
   SetPosition(Position);
   BuildLine();
}



void ALineBase::BuildLine()
{
   if(Mode == LineMode::segment) { RotateLaserLookAt(Position, Direction); }
   else                          { RotateLine(Direction); }

   if(     Mode == LineMode::line)    { SetLaserScale(Line, FVector(NULL, NULL, CoordinateSystem->MaxVisibleLength())); }
   else if(Mode == LineMode::segment) { ScaleLine(Line, UKismetMathLibrary::VSize(Direction - Position)); }
   else                               { ScaleVector(Line, Arrowhead, UKismetMathLibrary::VSize(Direction)); }
}

// Protected ----------------------------------------------------------------------------------------

void ALineBase::CreateVectorGuides(LaserColors color)
{
   AddVectorGuide(CoordinateSystem->AddVectorStruct(color, FVector::ZeroVector, Position, VectorStructMode::vector));
   AddVectorGuide(CoordinateSystem->AddVectorStruct(color, Position, Direction, VectorStructMode::vector));
}

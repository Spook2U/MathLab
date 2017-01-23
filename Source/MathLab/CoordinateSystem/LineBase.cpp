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

// Unreal Events -----------------------------------------------------------------------------------

void ALineBase::BeginPlay()
{
   Super::BeginPlay();
}



void ALineBase::SetComponents(TArray<UStaticMeshComponent*> components)
{
   for(UStaticMeshComponent *c : components)
   {
      if(c->GetName().Equals("Line"))      { this->Line      = c; }
      if(c->GetName().Equals("Arrowhead")) { this->Arrowhead = c; }
   }

   MLD_PTR_CHECK(Line);
   MLD_PTR_CHECK(Arrowhead);
   if(!(Line && Arrowhead)) return;

   ScaleLineInit(Line);
   ScaleArrowheadInit(Arrowhead);
   AddLaserComponent(Line);
   AddLaserComponent(Arrowhead);

   Arrowhead->SetHiddenInGame(true);

}



void ALineBase::SetValuesLine(ACoordinateSystemBase * coordinateSystem, LaserColors color, FVector position, FVector direction, LineMode mode)
{
   SetValues(coordinateSystem, color);
   this->Position = position;
   this->Direction = direction;
   this->Mode = mode;

   switch(Mode)
   {
      case LineMode::line:    CreateGuides(color); break;
      case LineMode::segment: break;
      case LineMode::vector:  Arrowhead->SetHiddenInGame(false); break;
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
   MLD_PTR_CHECK(Line);
   MLD_PTR_CHECK(Arrowhead);
   if(!(Line && Arrowhead)) return;

   //Make Rotation
   if(Mode == LineMode::segment) { RotateLine(Position, Direction); }
   else                          { RotateLine(Direction); }

   //Make Scale
   if(     Mode == LineMode::line)    { ScaleLaserLenght(Line, CoordinateSystem->MaxVisibleLength()); }
   else if(Mode == LineMode::segment) { ScaleLine(Line, UKismetMathLibrary::VSize(Direction - Position)); }
   else                               { ScaleVector(Line, Arrowhead, UKismetMathLibrary::VSize(Direction)); }
}

// Protected ----------------------------------------------------------------------------------------

void ALineBase::CreateGuides(LaserColors color)
{
   if(IsGuide) return;

   AddGuide(CoordinateSystem->AddLine(color, true, FVector(0, 0, 0), Position, LineMode::vector));
   AddGuide(CoordinateSystem->AddLine(color, true, Position, Direction, LineMode::vector));
}

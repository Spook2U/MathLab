// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "LineBase.h"

ALineBase::ALineBase()
{
   Position  = FVector();
   Direction = FVector();
}

// Unreal Events -----------------------------------------------------------------------------------

void ALineBase::BeginPlay()
{
   Super::BeginPlay();
}

// Initialise --------------------------------------------------------------------------------------

void ALineBase::SetComponents(TArray<UStaticMeshComponent*> components)
{
   for(UStaticMeshComponent *c : components)
   {
      if(c->GetName().Equals("Line"))      { this->Line      = c; }
      if(c->GetName().Equals("Arrowhead")) { this->Arrowhead = c; }
   }

   InitScaleLine(Line);
   InitScaleArrowhead(Arrowhead);
   AddLaserComponent(Line);
   AddLaserComponent(Arrowhead);

   Arrowhead->SetHiddenInGame(true);

}

// Update -------------------------------------------------------------------------------------------

void ALineBase::Update()
{
   Super::Update();
   SetPosition(Position);
   BuildLine();
}

// Setup --------------------------------------------------------------------------------------------

void ALineBase::BuildLine()
{
   //Make Rotation
   if(Mode == LineMode::segment) { SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CoordinateToLocation(Position), CoordinateToLocation(Direction))); }
   else                          { SetActorRotation(UKismetMathLibrary::Conv_VectorToRotator(Direction*FVector(1.f, -1.f, 1.f))); }

   //Make Scale
   if(     Mode == LineMode::line)    { Line->SetWorldScale3D(FVector(Line->GetComponentScale().X, Line->GetComponentScale().Y, CoordinateSystem->MaxVisibleLength())); }
   else if(Mode == LineMode::segment) { ScaleLaser(UKismetMathLibrary::VSize(Direction - Position)); }
   else                               { ScaleLaser(UKismetMathLibrary::VSize(Direction)); }
}

// -------------------------------------------------------------------------------------------------

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

// Protected ----------------------------------------------------------------------------------------

void ALineBase::CreateGuides(LaserColors color)
{
   if(IsGuide) return;

   AddGuide(CoordinateSystem->AddLine(color, true, FVector(0, 0, 0), Position, LineMode::vector));
   AddGuide(CoordinateSystem->AddLine(color, true, Position, Direction, LineMode::vector));
}

void ALineBase::ScaleLaser(float length)
{
   //Move line to the centerpoint between point A (diection) and B (position)
   MoveLaser(Line, length/2);
   if(Mode == LineMode::vector)
   {
      MoveLaser(Arrowhead, length-Size);
   }

   //Scale from Point A to B
   Line->SetWorldScale3D(FVector(Line->GetComponentScale().X, Line->GetComponentScale().Y, (CoordinateSystem->ConvertFactor/100)*length));
}

void ALineBase::MoveLaser(UStaticMeshComponent *laser, float length)
{
   laser->SetWorldLocation(((CoordinateSystem->ConvertFactor*length)*laser->GetUpVector()) + GetActorLocation());
}


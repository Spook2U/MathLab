// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "PointBase.h"

APointBase::APointBase()
{
   Coordinate = FVector();
}

void APointBase::BeginPlay() { Super::BeginPlay(); }



void APointBase::SetComponents(UStaticMeshComponent *point)
{
   this->Point = point;
   Point->SetWorldScale3D(Point->GetComponentScale()*Size);
}



void APointBase::Update()
{
   Super::Update();
   SetPosition(Coordinate);
}



void APointBase::SetValuesPoint(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector coordinate)
{
   SetValues(coordinateSystem, color);
   this->Coordinate = coordinate;
   if(!IsGuide)
   {
      CreateGuidesPoint(color);
      ShowGuides(false);
   }
}



void APointBase::CreateGuidesPoint(LaserColors color)
{
}

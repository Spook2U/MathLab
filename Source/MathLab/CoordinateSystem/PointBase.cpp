// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "PointBase.h"

void APointBase::ConstructPoint()
{
   Point->SetWorldScale3D(Point->GetComponentScale()*Size);
}

void APointBase::SetComponents(UStaticMeshComponent * point)
{
   this->Point = point;
}

void APointBase::SetValuesPoint(ACoordinateSystemBase * coordinateSystem, LaserColors color, FVector coordinate)
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

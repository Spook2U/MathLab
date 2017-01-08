// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "PointBase.h"

void APointBase::SetValuesPoint(ACoordinateSystemBase * coordinateSystem, LaserColors color, FVector coordinate)
{
   SetValues(coordinateSystem, color);
   this->Coordinate = coordinate;
   if(!isGuide)
   {
      CreateGuidesPoint(color);
      ShowGuides(false);
   }

   if(isGuide) {}
}

void APointBase::CreateGuidesPoint(LaserColors color)
{
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "GeometryBase.h"
#include "PointBase.h"
#include "UnitBase.h"
#include "CoordinateSystemBase.h"


ACoordinateSystemBase::ACoordinateSystemBase()      { PrimaryActorTick.bCanEverTick = true;}
void ACoordinateSystemBase::BeginPlay()             {	Super::BeginPlay(); }
void ACoordinateSystemBase::Tick( float DeltaTime ) { Super::Tick( DeltaTime ); }

void ACoordinateSystemBase::setAxis(UStaticMeshComponent * xAxis, UStaticMeshComponent * yAxis, UStaticMeshComponent * zAxis)
{
   this->XAxis = xAxis;
   this->YAxis = yAxis;
   this->ZAxis = zAxis;
}

void ACoordinateSystemBase::scaleAxis(float length, float diameter)
{
   ConvertFactor = AxisLength * 100 / UnitCount;

   FVector scaleVector = {diameter, diameter, 2 * length};

   XAxis->SetWorldScale3D(scaleVector);
   YAxis->SetWorldScale3D(scaleVector);
   ZAxis->SetWorldScale3D(scaleVector);
}

AGeometryBase *ACoordinateSystemBase::addGeometry(bool isGuide, TSubclassOf<AGeometryBase> geometry)
{
   AGeometryBase *newGeometry;

   FTransform transform = GetTransform();

   newGeometry = (AGeometryBase *)GetWorld()->SpawnActor(geometry, &transform);

   AttachToActor(newGeometry, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
   newGeometry->isGuide = isGuide;
   if(!isGuide) Elements.Add(newGeometry);

   return newGeometry;
}

void ACoordinateSystemBase::addUnits()
{
   for(int i = -UnitCount; i <= UnitCount; i++)
   {
      if(i != 0) // No Unit at Origin
      {
         addUnits_ToAxis(XAxis, i);
         addUnits_ToAxis(YAxis, i);
         addUnits_ToAxis(ZAxis, i);
      }
   }
}

void ACoordinateSystemBase::addUnits_ToAxis(UStaticMeshComponent * axis, int index)
{
   AUnitBase *newUnit;
   newUnit = (AUnitBase *)addGeometry(false, AUnitBase::StaticClass());

   newUnit->SetValuesPoint(this, LaserColor, axis->GetUpVector()*index);
   newUnit->OrientateToAxis(axis);
}



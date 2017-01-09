// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "GeometryBase.h"
#include "PointBase.h"
#include "UnitBase.h"
#include "CoordinateSystemBase.h"


ACoordinateSystemBase::ACoordinateSystemBase()      
{ 
   PrimaryActorTick.bCanEverTick = true;

   XAxis = NULL;
   YAxis = NULL;
   ZAxis = NULL;

   UnitCount = 0;
}

void ACoordinateSystemBase::BeginPlay()             
{	
   Super::BeginPlay(); 
}
void ACoordinateSystemBase::Tick( float DeltaTime ) { Super::Tick( DeltaTime ); }



void ACoordinateSystemBase::TestFunction()
{
   printLog("test %s", *FVector(1.f, 2.f, 3.f).ToString());
}



void ACoordinateSystemBase::SetAxis(UStaticMeshComponent *xAxis, UStaticMeshComponent *yAxis, UStaticMeshComponent *zAxis)
{
   if(xAxis && yAxis && zAxis)
   {
      this->XAxis = xAxis;
      this->YAxis = yAxis;
      this->ZAxis = zAxis;
   }
}

void ACoordinateSystemBase::ScaleAxis(float length, float diameter)
{
   if(UnitCount)
   {
      ConvertFactor = AxisLength * 100 / UnitCount;
   }

   FVector scaleVector = {diameter, diameter, 2 * length};

   XAxis->SetWorldScale3D(scaleVector);
   YAxis->SetWorldScale3D(scaleVector);
   ZAxis->SetWorldScale3D(scaleVector);
}

AGeometryBase *ACoordinateSystemBase::AddGeometry(bool isGuide, TSubclassOf<AGeometryBase> geometry)
{
   AGeometryBase *newGeometry;
   FTransform transform = GetTransform();

   newGeometry = (AGeometryBase *)GetWorld()->SpawnActor(geometry, &transform);

   AttachToActor(newGeometry, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
   newGeometry->IsGuide = isGuide;
   if(!isGuide) Elements.Add(newGeometry);

   return newGeometry;
}

void ACoordinateSystemBase::AddUnits()
{
   for(int i = UnitCount * (-1); i <= UnitCount; i++)
   {
      if(i != 0) // No Unit at Origin
      {
         //AddUnits_ToAxis(XAxis, i);
         //AddUnits_ToAxis(YAxis, i);
         //AddUnits_ToAxis(ZAxis, i);
      }
   }
}

void ACoordinateSystemBase::AddUnits_ToAxis(UStaticMeshComponent *axis, int index)
{
   if(!axis) return; /* nur bei ... */

   AUnitBase *newUnit = (AUnitBase *)AddGeometry(false, AUnitBase::StaticClass());

   if(newUnit)
   {
      newUnit->SetValuesPoint(this, LaserColor, axis->GetUpVector()*index);
      newUnit->OrientateToAxis(axis);
   }
}



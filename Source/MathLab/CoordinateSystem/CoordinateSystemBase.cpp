// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "GeometryBase.h"
#include "PointBase.h"
#include "UnitBase.h"
#include "CoordinateSystemBase.h"


ACoordinateSystemBase::ACoordinateSystemBase()      
{ 
   PrimaryActorTick.bCanEverTick = true;

   AxisLength = 1;
   AxisSize = 0.03f;
   UnitCount = 10;
   UnitSizeFactor = 0.5f;
   LaserSizefactor = 0.5f;
   LaserColor = LaserColors::green;
   Glowiness = 10.f;
   Elements;
   ConvertFactor = 0;
   MaxCoordinate = 0;

   //XAxis = NULL;
   //YAxis = NULL;
   //ZAxis = NULL;

   TArray<UStaticMeshComponent *> comps;
   GetComponents(comps);
   PRINTLOG("%d", comps.Num());
      
   UnitBP = NULL;
   static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/Unit.Unit'"));
   if(ItemBlueprint.Object) { UnitBP = (UClass *)ItemBlueprint.Object->GeneratedClass; }
   
}

void ACoordinateSystemBase::BeginPlay()             
{	
   Super::BeginPlay(); 
   TestFunction();
}
void ACoordinateSystemBase::Tick( float DeltaTime ) { Super::Tick( DeltaTime ); }



void ACoordinateSystemBase::TestFunction()
{
}



void ACoordinateSystemBase::SetAxis(UStaticMeshComponent *xAxis, UStaticMeshComponent *yAxis, UStaticMeshComponent *zAxis)
{
   POINTERTEST(xAxis);
   POINTERTEST(yAxis);
   POINTERTEST(zAxis);
   if(xAxis && yAxis && zAxis)
   {
      this->XAxis = xAxis;
      this->YAxis = yAxis;
      this->ZAxis = zAxis;
   }
}

void ACoordinateSystemBase::ScaleAxis(float length, float diameter)
{
   ConvertFactor = AxisLength * 100 / UnitCount;

   FVector scaleVector = {diameter, diameter, 2 * length};

   POINTERTEST(XAxis);
   POINTERTEST(YAxis);
   POINTERTEST(ZAxis);
   if(XAxis && YAxis && ZAxis)
   {
      XAxis->SetWorldScale3D(scaleVector);
      YAxis->SetWorldScale3D(scaleVector);
      ZAxis->SetWorldScale3D(scaleVector);
   }
}

AGeometryBase *ACoordinateSystemBase::AddGeometry(bool isGuide, TSubclassOf<AGeometryBase> geometry)
{
   AGeometryBase *newGeometry;
   FTransform transform = GetTransform();

   newGeometry = (AGeometryBase *)GetWorld()->SpawnActor(geometry, &transform);

   POINTERTEST(newGeometry);
   if(newGeometry)
   {
      newGeometry->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
      newGeometry->IsGuide = isGuide;
      if(!isGuide) Elements.Add(newGeometry);
   }
   return newGeometry;
}

void ACoordinateSystemBase::AddUnits()
{
   //for(int i = UnitCount * (-1); i <= UnitCount; i++)
   //{
   //   if(i != 0) // No Unit at Origin
   //   {
         POINTERTEST(XAxis);
         POINTERTEST(YAxis);
         POINTERTEST(ZAxis);
         if(XAxis && YAxis && ZAxis)
         {
            AddUnits_ToAxis(XAxis, 1);
            //AddUnits_ToAxis(XAxis, i);
            //AddUnits_ToAxis(YAxis, i);
            //AddUnits_ToAxis(ZAxis, i);
         }
   //   }
   //}
}

void ACoordinateSystemBase::AddUnits_ToAxis(UStaticMeshComponent *axis, int index)
{
   AUnitBase *newUnit = (AUnitBase *)AddGeometry(false, UnitBP);
   
   POINTERTEST(newUnit);
   if(newUnit)
   {
      newUnit->SetValuesPoint(this, LaserColor, axis->GetUpVector()*index);
      newUnit->OrientateToAxis(axis);
   }
}



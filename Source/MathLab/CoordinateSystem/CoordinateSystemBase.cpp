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
   LaserColor = LaserColors::green;

   UnitSizeFactor = 0.5f;
   LaserSizeFactor = 0.5f;
   Glowiness = 10.f;

   Elements;
   ConvertFactor = 0;
   MaxCoordinate = 0;

   XAxis = NULL;
   YAxis = NULL;
   ZAxis = NULL;

   UnitBP = NULL;
   static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/Unit.Unit'"));
   if(ItemBlueprint.Object) { UnitBP = (UClass *)ItemBlueprint.Object->GeneratedClass; }
   
}

void ACoordinateSystemBase::OnConstruction(const FTransform &Transform) { Super::OnConstruction(Transform); }

void ACoordinateSystemBase::PostInitProperties()
{
   Super::PostInitProperties();
   MaxCoordinate = UnitCount;
}

void ACoordinateSystemBase::BeginPlay()             
{	
#ifdef _UE_BUILD_DEBUG_FLAG_
   g_this = this;
#endif

   //TArray<UStaticMeshComponent *> comps;
   //GetComponents(comps);
   //for(UStaticMeshComponent *c : comps)
   //{
   //   POINTERTEST(c);
   //   if(c->GetName().Compare("X-Axis") == 0) { XAxis = c; }
   //   if(c->GetName().Compare("Y-Axis") == 0) { YAxis = c; }
   //   if(c->GetName().Compare("Z-Axis") == 0) { ZAxis = c; }
   //}
   //PRINTLOG("%s", *XAxis->GetName());
   
   InitCoordinateSystem();
   
   Super::BeginPlay();
   
   TestFunction();
}
void ACoordinateSystemBase::Tick( float DeltaTime ) { Super::Tick( DeltaTime ); }

void ACoordinateSystemBase::bp_debug_Screen(FString inString, FLinearColor color) { PRINTSCN(color.ToFColor(true), "%s", *inString); }



void ACoordinateSystemBase::TestFunction()
{
}



void ACoordinateSystemBase::InitCoordinateSystem_Implementation() {}

void ACoordinateSystemBase::SetComponents(UStaticMeshComponent *xAxis, UStaticMeshComponent *yAxis, UStaticMeshComponent *zAxis)
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
   if(UnitCount) ConvertFactor = AxisLength * 100 / UnitCount;

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
   for(int i = UnitCount * (-1); i <= UnitCount; i++)
   {
      if(i != 0) // No Unit at Origin
      {
         POINTERTEST(XAxis);
         POINTERTEST(YAxis);
         POINTERTEST(ZAxis);
         if(XAxis && YAxis && ZAxis)
         {
            AddUnits_ToAxis(XAxis, i);
            AddUnits_ToAxis(YAxis, i);
            AddUnits_ToAxis(ZAxis, i);
         }
      }
   }
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



// Extern Access for Actor to call EndPlay() in DebugTools
#ifdef _UE_BUILD_DEBUG_FLAG_
ACoordinateSystemBase *g_this = NULL; 
#endif


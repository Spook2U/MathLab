// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"

#include "GeometryBase.h"
#include "LineBase.h"
#include "PlaneBase.h"
#include "PointBase.h"
#include "SphereBase.h"
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
   static ConstructorHelpers::FObjectFinder<UBlueprint> UnitBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/Unit.Unit'"));
   if(UnitBlueprint.Object) { UnitBP = (UClass *)UnitBlueprint.Object->GeneratedClass; }

   PointBP = NULL;   
   static ConstructorHelpers::FObjectFinder<UBlueprint> PointBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/Point.Point'"));
   if(PointBlueprint.Object) { PointBP = (UClass *)PointBlueprint.Object->GeneratedClass; }

   LineBP = NULL;
   static ConstructorHelpers::FObjectFinder<UBlueprint> LineBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/Line.Line'"));
   if(LineBlueprint.Object) { LineBP = (UClass *)LineBlueprint.Object->GeneratedClass; }

   PlaneBP = NULL;
   static ConstructorHelpers::FObjectFinder<UBlueprint> PlaneBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/Plane.Plane'"));
   if(PlaneBlueprint.Object) { PlaneBP = (UClass *)PlaneBlueprint.Object->GeneratedClass; }

   SphereBP = NULL;
   static ConstructorHelpers::FObjectFinder<UBlueprint> SphereBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/Sphere.Sphere'"));
   if(SphereBlueprint.Object) { SphereBP = (UClass *)SphereBlueprint.Object->GeneratedClass; }
}

// Unreal Events -----------------------------------------------------------------------------------

void ACoordinateSystemBase::OnConstruction(const FTransform & Transform)
{
   MaxCoordinate = UnitCount;
}

void ACoordinateSystemBase::BeginPlay()
{	
#ifdef _UE_BUILD_DEBUG_FLAG_
   g_this = this;
#endif

   Super::BeginPlay();
   TestFunction();
}
void ACoordinateSystemBase::Tick( float DeltaTime ) { Super::Tick( DeltaTime ); }



void ACoordinateSystemBase::bp_debug_Screen(FString inString, FLinearColor color) { MLD_BLP(color.ToFColor(true), "%s", *inString); }



void ACoordinateSystemBase::TestFunction()
{
}

// Initialise --------------------------------------------------------------------------------------

void ACoordinateSystemBase::SetComponents(UStaticMeshComponent *xAxis, UStaticMeshComponent *yAxis, UStaticMeshComponent *zAxis)
{
   MLD_PTR_CHECK(xAxis);
   MLD_PTR_CHECK(yAxis);
   MLD_PTR_CHECK(zAxis);
   if(xAxis && yAxis && zAxis)
   {
      this->XAxis = xAxis;
      this->YAxis = yAxis;
      this->ZAxis = zAxis;
   }
}

// Pure Functions -----------------------------------------------------------------------------------

float ACoordinateSystemBase::MaxVisibleLength()
{
   float length;
   FVector v = FVector(1, 1, 1);

   length = (v*MaxCoordinate - v*MaxCoordinate*(-1)).Size();

   return length;
}

// Update -------------------------------------------------------------------------------------------

void ACoordinateSystemBase::Update()
{
   for(AGeometryBase *g : Elements)
   {
      MLD_PTR_CHECK(g);
      if(g)
      {
         g->Update();
      }
   }
}

// Setup --------------------------------------------------------------------------------------------

void ACoordinateSystemBase::ScaleAxis(float length, float diameter)
{
   if(UnitCount) ConvertFactor = AxisLength * 100 / UnitCount;

   FVector scaleVector = {diameter, diameter, 2 * length};

   MLD_PTR_CHECK(XAxis);
   MLD_PTR_CHECK(YAxis);
   MLD_PTR_CHECK(ZAxis);
   if(XAxis && YAxis && ZAxis)
   {
      XAxis->SetWorldScale3D(scaleVector);
      YAxis->SetWorldScale3D(scaleVector);
      ZAxis->SetWorldScale3D(scaleVector);
   }
}

// Make ---------------------------------------------------------------------------------------------

AGeometryBase *ACoordinateSystemBase::AddGeometry(bool isGuide, TSubclassOf<AGeometryBase> geometry)
{
   AGeometryBase *newGeometry;
   FTransform transform = GetTransform();

   newGeometry = (AGeometryBase *)GetWorld()->SpawnActor(geometry, &transform);

   MLD_PTR_CHECK(newGeometry);
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
         MLD_PTR_CHECK(XAxis);
         MLD_PTR_CHECK(YAxis);
         MLD_PTR_CHECK(ZAxis);
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
   
   MLD_PTR_CHECK(newUnit);
   if(newUnit)
   {
      newUnit->SetValuesPoint(this, LaserColor, axis->GetUpVector()*index);
      newUnit->OrientateToAxis(axis);
   }
}

APointBase *ACoordinateSystemBase::MakePoint(LaserColors color, FVector coordinate, bool showGuides)
{
   APointBase *point = AddPoint(color, false, coordinate);
   point->ShowGuides(showGuides);
   return point;
}

ALineBase *ACoordinateSystemBase::MakeLine(LaserColors color, FVector position, FVector direction, LineMode mode, bool showGuides)
{
   ALineBase *line = AddLine(color, false, position, direction, mode);
   line->ShowGuides(showGuides);
   return line;
}

APlaneBase *ACoordinateSystemBase::MakePlane(LaserColors color, FVector position, FVector direction1, FVector direction2, PlaneMode mode, bool showGuides)
{
   APlaneBase *plane = AddPlane(color, false, position, direction1, direction2, mode);
   plane->ShowGuides(showGuides);
   return plane;
}

ASphereBase *ACoordinateSystemBase::MakeSphere(LaserColors color, FVector position, float radius, bool showGuides)
{
   ASphereBase *sphere = AddSphere(color, false, position, radius);
   sphere->ShowGuides(showGuides);
   return sphere;
}

// --------------------------------------------------------------------------------------------------

APointBase *ACoordinateSystemBase::AddPoint(LaserColors color, bool isGuide, FVector coordinate)
{
   APointBase *newPoint = (APointBase *)AddGeometry(isGuide, PointBP);

   MLD_PTR_CHECK(newPoint);
   if(newPoint)
   {
      newPoint->SetValuesPoint(this, color, coordinate);
   }
   return newPoint;
}

ALineBase *ACoordinateSystemBase::AddLine(LaserColors color, bool isGuide, FVector position, FVector direction, LineMode mode)
{
   ALineBase *newLine = (ALineBase *)AddGeometry(isGuide, LineBP);

   MLD_PTR_CHECK(newLine);
   if(newLine)
   {
      newLine->SetValuesLine(this, color, position, direction, mode);
   }
   return newLine;
}

APlaneBase *ACoordinateSystemBase::AddPlane(LaserColors color, bool isGuide, FVector position, FVector direction1, FVector direction2, PlaneMode mode)
{
   APlaneBase *newPlane = (APlaneBase *)AddGeometry(isGuide, PlaneBP);

   MLD_PTR_CHECK(newPlane);
   if(newPlane)
   {
      newPlane->SetValuesPlane(this, color, position, direction1, direction2, mode);
   }
   return newPlane;
}

ASphereBase * ACoordinateSystemBase::AddSphere(LaserColors color, bool isGuide, FVector position, float radius)
{
   ASphereBase *newSphere = (ASphereBase *)AddGeometry(isGuide, SphereBP);

   MLD_PTR_CHECK(newSphere);
   if(newSphere)
   {
      newSphere->SetValuesSphere(this, color, position, radius);
   }
   return newSphere;
}



// Extern Access for Actor to call EndPlay() in DebugTools
#ifdef _UE_BUILD_DEBUG_FLAG_
ACoordinateSystemBase *g_this = NULL; 
#endif


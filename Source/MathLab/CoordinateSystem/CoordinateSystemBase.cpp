// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"

#include <typeinfo>

#include "GeometryBase.h"
#include "LineBase.h"
#include "PlaneBase.h"
#include "PointBase.h"
#include "SphereBase.h"
#include "UnitBase.h"
#include "VectorStruct.h"

#include "CoordinateSystemBase.h"
 
ACoordinateSystemBase::ACoordinateSystemBase()      
{ 
   PrimaryActorTick.bCanEverTick = true;

   AxisLength = 1;
   AxisSize = 0.03f;
   UnitCount = 10;
   LaserColor = LaserColors::green;

   UnitSizeFactor = 0.5f;
   LaserSizeFactor = 0.4f;
   Glowiness = 10.f;

   Elements;
   ConvertFactor = 0;
   MaxCoordinate = 0;

   XAxis = nullptr;
   YAxis = nullptr;
   ZAxis = nullptr;

   UnitBP = nullptr;
   static ConstructorHelpers::FObjectFinder<UBlueprint> UnitBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/Unit.Unit'"));
   if(UnitBlueprint.Object) { UnitBP = (UClass *)UnitBlueprint.Object->GeneratedClass; }

   PointBP = nullptr;   
   static ConstructorHelpers::FObjectFinder<UBlueprint> PointBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/Point.Point'"));
   if(PointBlueprint.Object) { PointBP = (UClass *)PointBlueprint.Object->GeneratedClass; }

   LineBP = nullptr;
   static ConstructorHelpers::FObjectFinder<UBlueprint> LineBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/Line.Line'"));
   if(LineBlueprint.Object) { LineBP = (UClass *)LineBlueprint.Object->GeneratedClass; }

   PlaneBP = nullptr;
   static ConstructorHelpers::FObjectFinder<UBlueprint> PlaneBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/Plane.Plane'"));
   if(PlaneBlueprint.Object) { PlaneBP = (UClass *)PlaneBlueprint.Object->GeneratedClass; }

   SphereBP = nullptr;
   static ConstructorHelpers::FObjectFinder<UBlueprint> SphereBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/Sphere.Sphere'"));
   if(SphereBlueprint.Object) { SphereBP = (UClass *)SphereBlueprint.Object->GeneratedClass; }

   VectorStructBP = nullptr;
   static ConstructorHelpers::FObjectFinder<UBlueprint> VectorStructBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/VectorStruct.VectorStruct'"));
   if(VectorStructBlueprint.Object) { VectorStructBP = (UClass *)VectorStructBlueprint.Object->GeneratedClass; }
}

// Unreal Events -----------------------------------------------------------------------------------

void ACoordinateSystemBase::OnConstruction(const FTransform &Transform)
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



void ACoordinateSystemBase::TestFunction()
{
   //FLinearEqualation le = FLinearEqualation(FNMatrix({FNVector({2, -4, 2}), FNVector({4, -6, 4}), FNVector({-4, -2, 4}), FNVector({-4, 0, 4})}));
   //FLinearEqualation le = FLinearEqualation(FNMatrix({FNVector({0, -4, 2}), FNVector({4, -6, 4}), FNVector({-4, -2, 4}), FNVector({-4, 0, 4})}));
   //FLinearEqualation le = FLinearEqualation(FNMatrix({FNVector({2, -4, 2}), FNVector({4, -8, 8}), FNVector({-4, -2, 4}), FNVector({-4, 0, 4})}));
   //FLinearEqualation le = FLinearEqualation(FNMatrix({FNVector({2, -4, 2, 2}), FNVector({4, -8, 4, 4}), FNVector({-4, -2, 4, 2}), FNVector({-4, 0, 4, 2}), FNVector({1, 1, 1, 2})}));
   //FLinearEqualation le = FLinearEqualation(FNMatrix({FNVector({0, 0, 0}), FNVector({4, -8, 4}), FNVector({-2, -2, 4}), FNVector({-4, 0, 4})}));

//   MLD_LOG("%s", *le.ToStringRows());

   //for(FString s : le.ToStringRows())
   //{
   //   MLD_LOG("%s", *s);
   //}
   //MLD_LOG("");

   //le.Solve();
   //
   //for(FString s : le.ToStringRows())
   //{
   //   MLD_LOG("%s", *s);
   //}
   //MLD_LOG("");

   //MLD_LOG("Result: %s", *le.GetResults().ToString());
   
}



void ACoordinateSystemBase::SetComponents(UStaticMeshComponent *xAxis, UStaticMeshComponent *yAxis, UStaticMeshComponent *zAxis)
{
   MLD_PTR_CHECK(xAxis);
   MLD_PTR_CHECK(yAxis);
   MLD_PTR_CHECK(zAxis);
   if(!(xAxis && yAxis && zAxis)) return;
   this->XAxis = xAxis;
   this->YAxis = yAxis;
   this->ZAxis = zAxis;
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
      MLD_PTR_CHECK(g); if(!g) return;
      g->Update();
   }
}

// Setup --------------------------------------------------------------------------------------------

void ACoordinateSystemBase::ScaleAxis(float length, float diameter)
{
   if(UnitCount) ConvertFactor = AxisLength * 100 / UnitCount;

   FVector scaleVector = {diameter, diameter, 2 * length};

   XAxis->SetWorldScale3D(scaleVector);
   YAxis->SetWorldScale3D(scaleVector);
   ZAxis->SetWorldScale3D(scaleVector);
}

// Make ---------------------------------------------------------------------------------------------

AGeometryBase *ACoordinateSystemBase::AddGeometry(TSubclassOf<AGeometryBase> geometry)
{  
   AGeometryBase *newGeometry;
   FTransform transform = GetTransform();

   newGeometry = (AGeometryBase *)GetWorld()->SpawnActor(geometry, &transform);

   MLD_PTR_CHECK(newGeometry); if(!newGeometry) return nullptr;
   newGeometry->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
   Elements.Add(newGeometry);
   
   return newGeometry;
}

void ACoordinateSystemBase::AddUnits()
{
   for(int i = UnitCount * (-1); i <= UnitCount; i++)
   {
      if(i != 0) // No Unit at Origin
      {
         AddUnits_ToAxis(XAxis, i);
         AddUnits_ToAxis(YAxis, i);
         AddUnits_ToAxis(ZAxis, i);
      }
   }
}

void ACoordinateSystemBase::AddUnits_ToAxis(UStaticMeshComponent *axis, int index)
{
   AUnitBase *newUnit = (AUnitBase *)AddGeometry(UnitBP);
   MLD_PTR_CHECK(newUnit); if(!newUnit) return;
   newUnit->InitUnit(this, LaserColor, axis->GetUpVector()*index);
   newUnit->OrientateToAxis(axis);
}

APointBase *ACoordinateSystemBase::MakePoint(LaserColors color, FVector coordinate, bool showGuides)
{  
   APointBase *point = (APointBase *)AddGeometry(PointBP);
   
   MLD_PTR_CHECK(point); if(!point) return nullptr;
   point->InitPoint(this, color, coordinate);
   point->ShowVectorGuides(showGuides);
   return point;
}

ALineBase *ACoordinateSystemBase::MakeLine(LaserColors color, FVector position, FVector direction, LineMode mode, bool showGuides)
{
   ALineBase *line = (ALineBase *)AddGeometry(LineBP);

   MLD_PTR_CHECK(line); if(!line) return nullptr;
   line->InitLine(this, color, position, direction, mode);
   line->ShowVectorGuides(showGuides);
   return line;
}

APlaneBase *ACoordinateSystemBase::MakePlane(LaserColors color, FVector position, FVector direction1, FVector direction2, PlaneMode mode, bool showGuides)
{
   APlaneBase *plane = (APlaneBase *)AddGeometry(PlaneBP);
   MLD_PTR_CHECK(plane); if(!plane) return nullptr;
   plane->InitPlane(this, color, position, direction1, direction2, mode);
   plane->ShowVectorGuides(showGuides);
   return plane;
}

ASphereBase *ACoordinateSystemBase::MakeSphere(LaserColors color, FVector coordinate, float radius, bool showGuides)
{
   ASphereBase *sphere = (ASphereBase *)AddGeometry(SphereBP);
   MLD_PTR_CHECK(sphere); if(!sphere) return nullptr;
   sphere->InitSphere(this, color, coordinate, radius);
   sphere->ShowVectorGuides(showGuides);
   return sphere;
}

// --------------------------------------------------------------------------------------------------

APointBase *ACoordinateSystemBase::AddPoint(LaserColors color, bool showGuides, FVector coordinate)
{  
   APointBase *newPoint = (APointBase *)AddGeometry(PointBP);
   MLD_PTR_CHECK(newPoint); if(!newPoint) return nullptr;
   newPoint->InitPoint(this, color, coordinate);
   newPoint->ShowVectorGuides(showGuides);
   return newPoint;
}

ALineBase *ACoordinateSystemBase::AddLine(LaserColors color, bool showGuides, FVector position, FVector direction, LineMode mode)
{
   ALineBase *newLine = (ALineBase *)AddGeometry(LineBP);
   MLD_PTR_CHECK(newLine); if(!newLine) return nullptr;
   newLine->InitLine(this, color, position, direction, mode);
   newLine->ShowVectorGuides(showGuides);
   return newLine;
}

APlaneBase *ACoordinateSystemBase::AddPlane(LaserColors color, bool showGuides, FVector position, FVector direction1, FVector direction2, PlaneMode mode)
{
   APlaneBase *newPlane = (APlaneBase *)AddGeometry(PlaneBP);
   MLD_PTR_CHECK(newPlane); if(!newPlane) return nullptr;
   newPlane->InitPlane(this, color, position, direction1, direction2, mode);
   newPlane->ShowVectorGuides(showGuides);
   return newPlane;
}

ASphereBase * ACoordinateSystemBase::AddSphere(LaserColors color, bool showGuides, FVector coordinate, float radius)
{
   ASphereBase *newSphere = (ASphereBase *)AddGeometry(SphereBP);
   MLD_PTR_CHECK(newSphere); if(!newSphere) return nullptr; 
   newSphere->InitSphere(this, color, coordinate, radius);
   newSphere->ShowVectorGuides(showGuides);
   return newSphere;
}

AVectorStruct *ACoordinateSystemBase::AddVectorStruct(LaserColors color, FVector pointA, FVector pointB, VectorStructMode mode)
{
   AVectorStruct *newVectorStruct = (AVectorStruct *)AddGeometry(VectorStructBP);
   MLD_PTR_CHECK(newVectorStruct); if(!newVectorStruct) return nullptr;
   newVectorStruct->SetValuesVectorStruct(this, color, pointA, pointB, mode);
   switch(mode)
   {
      case VectorStructMode::point:       newVectorStruct->SetVisibility(false, true, false, false); break;
      case VectorStructMode::segment:     newVectorStruct->SetVisibility(false, false, true, false); break;
      case VectorStructMode::vector:      newVectorStruct->SetVisibility(false, false, true, true);  break;
      case VectorStructMode::vectorPoint: newVectorStruct->SetVisibility(false, true, true, true);   break;
      case VectorStructMode::general:     newVectorStruct->SetVisibilityForAll(false);               break;
   }
   return newVectorStruct;
}



void ACoordinateSystemBase::bp_debug_Screen(FString inString, FLinearColor color) 
{ 
   MLD_BLP(color.ToFColor(true), "%s", *inString); 
}

FString ACoordinateSystemBase::FNVectorToString(FNVector inNVector)
{
   return inNVector.ToString();
}

FString ACoordinateSystemBase::FNMatrixToString(FNMatrix inNMatrix)
{
   return inNMatrix.ToString();
}

FString ACoordinateSystemBase::FLinearEqualationToString(FLinearEqualation inLinearEqualation)
{
   return inLinearEqualation.ToString();
}

void ACoordinateSystemBase::LE_Solve(FLinearEqualation inLinearEqualation)
{
   inLinearEqualation.Solve();
}



// Extern Access for Actor to call EndPlay() in DebugTools
#ifdef _UE_BUILD_DEBUG_FLAG_
ACoordinateSystemBase *g_this = NULL; 
#endif


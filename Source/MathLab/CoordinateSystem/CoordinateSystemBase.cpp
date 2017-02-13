// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "CoordinateSystemBase.h"

#include "GeometryBase.h"


 
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

   MLD_LOG("");
   MLD_LOG("===================");
   MLD_LOG("BeginPlay: %s", *UKismetMathLibrary::Now().ToString().Right(8));
   MLD_LOG("===================");
   MLD_LOG("");

   Super::BeginPlay();
   
   TestFunction();
}
void ACoordinateSystemBase::Tick( float DeltaTime ) { Super::Tick( DeltaTime ); }



void ACoordinateSystemBase::TestFunction()
{
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

APointBase *ACoordinateSystemBase::AddPoint(LaserColors color, FMathPoint inPoint, bool showGuides)
{  
   APointBase *point = (APointBase *)AddGeometry(PointBP);
   
   MLD_PTR_CHECK(point); if(!point) return nullptr;
   point->InitPoint(this, color, inPoint);
   point->ShowVectorGuides(showGuides);
   return point;
}

ALineBase *ACoordinateSystemBase::AddLine(LaserColors color, FMathLine inLine, LineMode mode, bool showGuides)
{
   if(inLine.Direction.Size() == 0) { MLD_ERR("Line not created. Invalid diection. Direction of the line cannt be {0, 0, 0}."); return nullptr; }

   ALineBase *line = (ALineBase *)AddGeometry(LineBP);

   MLD_PTR_CHECK(line); if(!line) return nullptr;
   line->InitLine(this, color, inLine, mode);
   line->ShowVectorGuides(showGuides);
   return line;
}

APlaneBase *ACoordinateSystemBase::AddPlane(LaserColors color, FMathPlane inPlane, PlaneMode mode, bool showGuides)
{
   if((inPlane.Direction1.Size() == 0) || (inPlane.Direction2.Size() == 0)) { MLD_ERR("Plane not created. Invalid diection. No direction of the plane shall be {0, 0, 0}."); return nullptr; }

   APlaneBase *plane = (APlaneBase *)AddGeometry(PlaneBP);
   MLD_PTR_CHECK(plane); if(!plane) return nullptr;
   plane->InitPlane(this, color, inPlane, mode);
   plane->ShowVectorGuides(showGuides);
   return plane;
}

ASphereBase *ACoordinateSystemBase::AddSphere(LaserColors color, FMathSphere inSphere, bool showGuides)
{
   if(inSphere.Radius == 0) { MLD_ERR("Sphere not created. Invalid radius. Cannt create Sphere with radius = 0."); return nullptr; }

   ASphereBase *sphere = (ASphereBase *)AddGeometry(SphereBP);
   MLD_PTR_CHECK(sphere); if(!sphere) return nullptr;
   sphere->InitSphere(this, color, inSphere);
   sphere->ShowVectorGuides(showGuides);
   return sphere;
}

AVectorStruct *ACoordinateSystemBase::AddVectorStruct(LaserColors color, FVector pointA, FVector pointB, VectorStructMode mode)
{
   AVectorStruct *newVectorStruct = (AVectorStruct *)AddGeometry(VectorStructBP);
   MLD_PTR_CHECK(newVectorStruct); if(!newVectorStruct) return nullptr;
   newVectorStruct->SetValuesVectorStruct(this, color, pointA, pointB, mode);
   switch(mode)
   {
      case VectorStructMode::point:       newVectorStruct->SetVisibility(false, true,  false, false); break;
      case VectorStructMode::segment:     newVectorStruct->SetVisibility(false, false, true,  false); break;
      case VectorStructMode::vector:      newVectorStruct->SetVisibility(false, false, true,  true);  break;
      case VectorStructMode::vectorPoint: newVectorStruct->SetVisibility(false, true,  true,  true);  break;
      case VectorStructMode::general:     newVectorStruct->SetVisibilityForAll(false);                break;
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

FString ACoordinateSystemBase::FLinearEqualationToString(FLinearSystem inLinearEqualation)
{
   return inLinearEqualation.ToString();
}

void ACoordinateSystemBase::LE_Solve(FLinearSystem inLinearEqualation)
{
   inLinearEqualation.GetSolution();
}

FString ACoordinateSystemBase::FPointToString(FMathPoint inPoint)
{
   return inPoint.Coordinate.ToString();
}

APointBase *ACoordinateSystemBase::FPointToPointBP(FMathPoint inPoint)
{
   return AddPoint(LaserColor, inPoint, false);
}

FMathPoint ACoordinateSystemBase::PointBPToFPoint(APointBase *inPoint)
{
   if(!MLD_PTR_CHECK(inPoint)) return FMathPoint();
   return inPoint->point;
}



float ACoordinateSystemBase::Distance(AGeometryBase *from, AGeometryBase *to)
{
   if(!(MLD_PTR_CHECK(from) && MLD_PTR_CHECK(to))) return 0.f;

   GeometryCalc calc;
   return calc.GetDistance(from, to);
}

FRelativePosition ACoordinateSystemBase::GetRelativePosition(AGeometryBase *from, AGeometryBase *to)
{
   if(!(MLD_PTR_CHECK(from) && MLD_PTR_CHECK(to))) return FRelativePosition(Relation::notSolved);

   GeometryCalc calc;
   return calc.GetRelativePosition(from, to);
}



// Extern Access for Actor to call EndPlay() in DebugTools
#ifdef _UE_BUILD_DEBUG_FLAG_
ACoordinateSystemBase *g_this = NULL; 
#endif


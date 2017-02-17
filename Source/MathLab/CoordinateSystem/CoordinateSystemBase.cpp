// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "CoordinateSystemBase.h"

#include "GeometryBase.h"


 
ACoordinateSystemBase::ACoordinateSystemBase()      
{ 
   PrimaryActorTick.bCanEverTick = true;

   axisLength = 1;
   axisSize = 0.03f;
   unitCount = 10;
   laserColor = LaserColors::green;

   unitSizeFactor = 0.5f;
   laserSizeFactor = 0.4f;
   glowiness = 1.f;

   elements;
   convertFactor = 0;
   maxCoordinate = 0;

   xAxis = nullptr;
   yAxis = nullptr;
   zAxis = nullptr;

   circleBP = nullptr;
   static ConstructorHelpers::FObjectFinder<UBlueprint> CircleBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/Circle.Circle'"));
   if(CircleBlueprint.Object) { circleBP = (UClass *)CircleBlueprint.Object->GeneratedClass; }

   lineBP = nullptr;
   static ConstructorHelpers::FObjectFinder<UBlueprint> LineBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/Line.Line'"));
   if(LineBlueprint.Object) { lineBP = (UClass *)LineBlueprint.Object->GeneratedClass; }

   planeBP = nullptr;
   static ConstructorHelpers::FObjectFinder<UBlueprint> PlaneBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/Plane.Plane'"));
   if(PlaneBlueprint.Object) { planeBP = (UClass *)PlaneBlueprint.Object->GeneratedClass; }

   pointBP = nullptr;   
   static ConstructorHelpers::FObjectFinder<UBlueprint> PointBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/Point.Point'"));
   if(PointBlueprint.Object) { pointBP = (UClass *)PointBlueprint.Object->GeneratedClass; }

   sphereBP = nullptr;
   static ConstructorHelpers::FObjectFinder<UBlueprint> SphereBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/Sphere.Sphere'"));
   if(SphereBlueprint.Object) { sphereBP = (UClass *)SphereBlueprint.Object->GeneratedClass; }

   unitBP = nullptr;
   static ConstructorHelpers::FObjectFinder<UBlueprint> UnitBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/Unit.Unit'"));
   if(UnitBlueprint.Object) { unitBP = (UClass *)UnitBlueprint.Object->GeneratedClass; }

   vectorStructBP = nullptr;
   static ConstructorHelpers::FObjectFinder<UBlueprint> VectorStructBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/VectorStruct.VectorStruct'"));
   if(VectorStructBlueprint.Object) { vectorStructBP = (UClass *)VectorStructBlueprint.Object->GeneratedClass; }
}

// Unreal Events -----------------------------------------------------------------------------------

void ACoordinateSystemBase::OnConstruction(const FTransform &Transform)
{
   maxCoordinate = unitCount;
}

void ACoordinateSystemBase::BeginPlay()
{	
#ifdef _UE_BUILD_DEBUG_FLAG_
   g_this = this;
#endif

   MLD_LOG("BeginPlay: %02i:%02i:%02i", UKismetMathLibrary::Now().GetHour(), UKismetMathLibrary::Now().GetMinute(),UKismetMathLibrary::Now().GetSecond());
   MLD_LOG("");

   Super::BeginPlay();
   
   TestFunction();
}
void ACoordinateSystemBase::Tick( float DeltaTime ) { Super::Tick( DeltaTime ); }



void ACoordinateSystemBase::TestFunction()
{
}



void ACoordinateSystemBase::SetComponents(UStaticMeshComponent *inXAxis, UStaticMeshComponent *inYAxis, UStaticMeshComponent *inZAxis)
{
   MLD_PTR_CHECK(inXAxis);
   MLD_PTR_CHECK(inYAxis);
   MLD_PTR_CHECK(inZAxis);
   if(!(inXAxis && inYAxis && inZAxis)) return;
   xAxis = inXAxis;
   yAxis = inYAxis;
   zAxis = inZAxis;
}

// Pure Functions -----------------------------------------------------------------------------------

float ACoordinateSystemBase::MaxVisibleLength()
{
   float length;
   FVector v = FVector(1, 1, 1);

   length = (v*maxCoordinate - v*maxCoordinate*(-1)).Size();

   return length;
}

// Update -------------------------------------------------------------------------------------------

void ACoordinateSystemBase::Update()
{
   for(AGeometryBase *g : elements)
   {
      MLD_PTR_CHECK(g); if(!g) return;
      g->Update();
   }
}

// Setup --------------------------------------------------------------------------------------------

void ACoordinateSystemBase::ScaleAxis(float length, float diameter)
{
   if(unitCount) convertFactor = axisLength * 100 / unitCount;

   FVector scaleVector = {diameter, diameter, 2 * length};

   xAxis->SetWorldScale3D(scaleVector);
   yAxis->SetWorldScale3D(scaleVector);
   zAxis->SetWorldScale3D(scaleVector);
}

// Make ---------------------------------------------------------------------------------------------

AGeometryBase *ACoordinateSystemBase::AddGeometry(TSubclassOf<AGeometryBase> geometry)
{  
   AGeometryBase *newGeometry;
   FTransform transform = GetTransform();

   newGeometry = (AGeometryBase *)GetWorld()->SpawnActor(geometry, &transform);

   MLD_PTR_CHECK(newGeometry); if(!newGeometry) return nullptr;
   newGeometry->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
   elements.Add(newGeometry);
   
   return newGeometry;
}

void ACoordinateSystemBase::AddUnits()
{
   for(int i = unitCount * (-1); i <= unitCount; i++)
   {
      if(i != 0) // No Unit at Origin
      {
         AddUnits_ToAxis(xAxis, i);
         AddUnits_ToAxis(yAxis, i);
         AddUnits_ToAxis(zAxis, i);
      }
   }
}

void ACoordinateSystemBase::AddUnits_ToAxis(UStaticMeshComponent *axis, int index)
{
   AUnitBase *newUnit = (AUnitBase *)AddGeometry(unitBP);
   MLD_PTR_CHECK(newUnit); if(!newUnit) return;
   newUnit->InitUnit(this, laserColor, axis->GetUpVector()*index);
   newUnit->OrientateToAxis(axis);
}

APointBase *ACoordinateSystemBase::AddPoint(LaserColors color, FMathPoint inPoint, bool showGuides)
{  
   APointBase *point = (APointBase *)AddGeometry(pointBP);
   
   MLD_PTR_CHECK(point); if(!point) return nullptr;
   point->InitPoint(this, color, inPoint);
   point->ShowVectorGuides(showGuides);
   return point;
}

ALineBase *ACoordinateSystemBase::AddLine(LaserColors color, FMathLine inLine, LineMode mode, bool showGuides)
{
   if(inLine.direction.Size() == 0) { MLD_ERR("Line not created. Invalid diection. Direction of the line cannt be {0, 0, 0}."); return nullptr; }

   ALineBase *line = (ALineBase *)AddGeometry(lineBP);

   MLD_PTR_CHECK(line); if(!line) return nullptr;
   line->InitLine(this, color, inLine, mode);
   line->ShowVectorGuides(showGuides);
   return line;
}

APlaneBase *ACoordinateSystemBase::AddPlane(LaserColors color, FMathPlane inPlane, PlaneMode mode, bool showGuides)
{
   if((inPlane.direction1.Size() == 0) || (inPlane.direction2.Size() == 0)) { MLD_ERR("Plane not created. Invalid diection. No direction of the plane shall be {0, 0, 0}."); return nullptr; }

   APlaneBase *plane = (APlaneBase *)AddGeometry(planeBP);
   MLD_PTR_CHECK(plane); if(!plane) return nullptr;
   plane->InitPlane(this, color, inPlane, mode);
   plane->ShowVectorGuides(showGuides);
   return plane;
}

ASphereBase *ACoordinateSystemBase::AddSphere(LaserColors color, FMathSphere inSphere, bool showGuides)
{
   if(inSphere.radius <= 0) { MLD_ERR("Sphere not created. Invalid radius. Cannt create Sphere with radius <= 0."); return nullptr; }

   ASphereBase *sphere = (ASphereBase *)AddGeometry(sphereBP);
   MLD_PTR_CHECK(sphere); if(!sphere) return nullptr;
   sphere->InitSphere(this, color, inSphere);
   sphere->ShowVectorGuides(showGuides);
   return sphere;
}

ACircleBase *ACoordinateSystemBase::AddCircle(LaserColors color, FMathCircle inCircle, bool showGuides)
{
   if(inCircle.radius <= 0) { MLD_ERR("Circle not created. Invalid radius. Cannt create Circle with radius <= 0."); return nullptr; }

   ACircleBase *circle = (ACircleBase *)AddGeometry(circleBP);
   MLD_PTR_CHECK(circle); if(!circle) return nullptr;
   circle->InitCircle(this, color, inCircle);
   circle->ShowVectorGuides(showGuides);
   return circle;
}

AVectorStruct *ACoordinateSystemBase::AddVectorStruct(LaserColors color, FVector pointA, FVector pointB, VectorStructMode mode)
{
   AVectorStruct *newVectorStruct = (AVectorStruct *)AddGeometry(vectorStructBP);
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
   return inPoint.coordinate.ToString();
}

APointBase *ACoordinateSystemBase::FPointToPointBP(FMathPoint inPoint)
{
   return AddPoint(laserColor, inPoint, false);
}

FMathPoint ACoordinateSystemBase::PointBPToFPoint(APointBase *inPoint)
{
   if(!MLD_PTR_CHECK(inPoint)) return FMathPoint();
   return inPoint->point;
}



float ACoordinateSystemBase::Distance(AGeometryBase *from, AGeometryBase *to)
{
   if(!(MLD_PTR_CHECK(from) && MLD_PTR_CHECK(to))) return 0.f;

   return m.GetDistance(from, to);
}

FRelativePosition ACoordinateSystemBase::GetRelativePosition(AGeometryBase *from, AGeometryBase *to)
{
   if(!(MLD_PTR_CHECK(from) && MLD_PTR_CHECK(to))) return Relation::undefined;

   return m.GetRelativePosition(from, to);
}



// Extern Access for Actor to call EndPlay() in DebugTools
#ifdef _UE_BUILD_DEBUG_FLAG_
ACoordinateSystemBase *g_this = NULL; 
#endif


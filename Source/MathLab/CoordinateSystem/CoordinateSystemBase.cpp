// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "CoordinateSystemBase.h"

#include "GeometryBase.h"


 
ACoordinateSystemBase::ACoordinateSystemBase()      
{ 
   PrimaryActorTick.bCanEverTick = true;

// Coordinate System ---------------------------------------------------------------------------------------------------------------------------------
   axisLength = 1;
   axisSize = 0.02f;

   glowiness = 10.f;

// Coordinate System - Units--------------------------------------------------------------------------------------------------------------------------

   unitCount = 10;
   unitLaserColor = LaserColors::green;
   unitGlowiness = 10.f;
   unitSizeFactor = 0.4f;
   unitLaserSizeFactor = 0.5f;

// Text ----------------------------------------------------------------------------------------------------------------------------------------------
   showNames = true;
   showMathData = true;
   nameTextSize = 7.5f;
   unitTextSize = 10;

   showCVectorName = false;
   showCVectorMathData = false;
   cVectorTextSize = 5;
   //textGlowiness = 5;

// Member --------------------------------------------------------------------------------------------------------------------------------------------
   elements;
   convertFactor = 0;
   maxCoordinate = 0;

   circleCounter      = 1;
   lineCounter        = 1;
   planeCounter       = 1;
   pointCounter       = 1;
   sphereCounter      = 1;
   unitCounter        = 1;
   constVectorCounter = 1;
   geometryCounter    = 1;


// Private -------------------------------------------------------------------------------------------------------------------------------------------
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

   cVectorBP = nullptr;
   static ConstructorHelpers::FObjectFinder<UBlueprint> CVectorBlueprint(TEXT("Blueprint'/Game/MathLab/Blueprints/CorrdinateSystem/CVector.CVector'"));
   if(CVectorBlueprint.Object) { cVectorBP = (UClass *)CVectorBlueprint.Object->GeneratedClass; }
}

// Unreal Events -------------------------------------------------------------------------------------------------------------------------------------

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



// Test Function -------------------------------------------------------------------------------------------------------------------------------------

void ACoordinateSystemBase::TestFunction()
{
}



// Coordinate System Setup ---------------------------------------------------------------------------------------------------------------------------

void ACoordinateSystemBase::SetComponents(UStaticMeshComponent *inXAxis, UStaticMeshComponent *inYAxis, UStaticMeshComponent *inZAxis)
{
   if(!(MLD_PTR_CHECK(inXAxis) && MLD_PTR_CHECK(inYAxis) && MLD_PTR_CHECK(inZAxis))) return;
   xAxis = inXAxis;
   yAxis = inYAxis;
   zAxis = inZAxis;
}

void ACoordinateSystemBase::ScaleAxis(float length, float diameter)
{
   if(!(MLD_PTR_CHECK(xAxis) && MLD_PTR_CHECK(yAxis) && MLD_PTR_CHECK(zAxis))) return;

   if(unitCount) { convertFactor = axisLength * 100 / unitCount; }

   FVector scaleVector = {diameter, diameter, 2 * length};

   xAxis->SetWorldScale3D(scaleVector);
   yAxis->SetWorldScale3D(scaleVector);
   zAxis->SetWorldScale3D(scaleVector);
}

void ACoordinateSystemBase::Update()
{
   for(AGeometryBase *g : elements)
   {
      if(!MLD_PTR_CHECK(g)) return;
      g->Update();
   }
}

// Add Functions--------------------------------------------------------------------------------------------------------------------------------------

AGeometryBase *ACoordinateSystemBase::AddGeometry(TSubclassOf<AGeometryBase> geometry, AActor *parent)
{  
   AGeometryBase *newGeometry;
   FTransform transform = GetTransform();

   newGeometry = (AGeometryBase *)GetWorld()->SpawnActor(geometry, &transform);

   if(!MLD_PTR_CHECK(newGeometry)) return nullptr;
   newGeometry->AttachToActor(parent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
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
   if(!MLD_PTR_CHECK(axis)) return;
   AUnitBase *newUnit = (AUnitBase *)AddGeometry(unitBP, this);
   
   if(!MLD_PTR_CHECK(newUnit)) return;
   newUnit->Init(this, unitLaserColor, axis->GetUpVector()*index, FString::Printf(TEXT("%d"), index));
   newUnit->OrientateToAxis(axis);
   newUnit->Update();
}

APointBase *ACoordinateSystemBase::AddPoint(LaserColors color, FMathPoint inPoint, FString inName, bool showGuides)
{  
   APointBase *point = (APointBase *)AddGeometry(pointBP, this);

   if(!MLD_PTR_CHECK(point)) return nullptr;
   point->Init(this, color, inPoint, inName);
   point->ShowConstructingVector(showGuides);
   point->Update();
   return point;
}

ALineBase *ACoordinateSystemBase::AddLine(LaserColors color, FMathLine inLine, LineMode mode, FString inName, bool showGuides)
{
   if(inLine.direction.Size() == 0) { MLD_ERR("Line not created. Invalid diection. Direction of the line cannt be {0, 0, 0}."); return nullptr; }

   ALineBase *line = (ALineBase *)AddGeometry(lineBP, this);

   if(!MLD_PTR_CHECK(line)) return nullptr;
   line->Init(this, color, inLine, mode, inName);
   line->ShowConstructingVector(showGuides);
   line->Update();
   return line;
}

APlaneBase *ACoordinateSystemBase::AddPlane(LaserColors color, FMathPlane inPlane, PlaneMode mode, FString inName, bool showGuides)
{
   if((inPlane.direction1.Size() == 0) || (inPlane.direction2.Size() == 0)) { MLD_ERR("Plane not created. Invalid diection. No direction of the plane shall be {0, 0, 0}."); return nullptr; }

   APlaneBase *plane = (APlaneBase *)AddGeometry(planeBP, this);
   if(!MLD_PTR_CHECK(plane)) return nullptr;
   plane->Init(this, color, inPlane, mode, inName);
   plane->ShowConstructingVector(showGuides);
   plane->Update();
   return plane;
}

ASphereBase *ACoordinateSystemBase::AddSphere(LaserColors color, FMathSphere inSphere, FString inName, bool showGuides)
{
   if(inSphere.radius <= 0) { MLD_ERR("Sphere not created. Invalid radius. Cannt create Sphere with radius <= 0."); return nullptr; }

   ASphereBase *sphere = (ASphereBase *)AddGeometry(sphereBP, this);
   if(!MLD_PTR_CHECK(sphere)) return nullptr;
   sphere->Init(this, color, inSphere, inName);
   sphere->ShowConstructingVector(showGuides);
   sphere->Update();
   return sphere;
}

ACircleBase *ACoordinateSystemBase::AddCircle(LaserColors color, FMathCircle inCircle, FString inName, bool showGuides)
{
   if(inCircle.radius <= 0) { MLD_ERR("Circle not created. Invalid radius. Cannt create Circle with radius <= 0."); return nullptr; }

   ACircleBase *circle = (ACircleBase *)AddGeometry(circleBP, this);
   if(!MLD_PTR_CHECK(circle)) return nullptr;
   circle->Init(this, color, inCircle, inName);
   circle->ShowConstructingVector(showGuides);
   circle->Update();
   return circle;
}

ACVectorBase *ACoordinateSystemBase::AddCVector(AActor *parent, LaserColors color, FVector pointA, FVector pointB, CVectorMode mode, FString inName)
{
   ACVectorBase *newCVector = (ACVectorBase *)AddGeometry(cVectorBP, parent);
   if(!MLD_PTR_CHECK(newCVector)) return nullptr;
   newCVector->Init(this, color, pointA, pointB, mode, inName);
   newCVector->Update();
   switch(mode)
   {
      case CVectorMode::point:       newCVector->SetVisibility(false, true,  false, false); break;
      case CVectorMode::segment:     newCVector->SetVisibility(false, false, true,  false); break;
      case CVectorMode::vector:      newCVector->SetVisibility(false, false, true,  true);  break;
      case CVectorMode::vectorPoint: newCVector->SetVisibility(false, true,  true,  true);  break;
      case CVectorMode::general:     newCVector->SetVisibilityForAll(false);                break;
   }
   return newCVector;
}

// Find Functions-------------------------------------------------------------------------------------------------------------------------------------

AGeometryBase* ACoordinateSystemBase::FindGeometryWithName(FString inName, bool &found)
{
   AGeometryBase *wanted = nullptr;
   found = false;

   for(AGeometryBase *g : elements)
   {
      if(!MLD_PTR_CHECK(g)) return nullptr;
      if(g->type == GeometryType::cVector) { continue; }
      if(g->type == GeometryType::unit)    { continue; }

      if(g->GetName() == inName)
      {
         found = true;
         wanted = g;
         break;
      }
   }
   return wanted;
}

TArray<ACircleBase *> ACoordinateSystemBase::FindCircle(FMathCircle inCircle, bool &found)
{
   TArray<ACircleBase *> wanted = TArray<ACircleBase*>();
   found = false;

   for(AGeometryBase *g : elements)
   {
      if(!MLD_PTR_CHECK(g)) return TArray<ACircleBase*>();
      if(g->type != GeometryType::circle) { continue; }

      ACircleBase *e = ((ACircleBase *)g);
      if(e->circle == inCircle)
      {
         found = true;
         wanted.Add(e);
      }
   }   
   return wanted;
}

TArray<ALineBase *> ACoordinateSystemBase::FindLine(FMathLine inLine, bool &found)
{
   TArray<ALineBase *> wanted = TArray<ALineBase*>();
   found = false;

   for(AGeometryBase *g : elements)
   {
      if(!MLD_PTR_CHECK(g)) return TArray<ALineBase*>();
      if(g->type != GeometryType::line) { continue; }

      ALineBase *e = ((ALineBase *)g);
      if(e->line == inLine)
      {
         found = true;
         wanted.Add(e);
      }
   }   
   return wanted;
}

TArray<APlaneBase *> ACoordinateSystemBase::FindPlane(FMathPlane inPlane, bool &found)
{
   TArray<APlaneBase *> wanted = TArray<APlaneBase*>();
   found = false;

   for(AGeometryBase *g : elements)
   {
      if(!MLD_PTR_CHECK(g)) return TArray<APlaneBase*>();
      if(g->type != GeometryType::plane) { continue; }

      APlaneBase *e = ((APlaneBase *)g);
      if(e->plane == inPlane)
      {
         found = true;
         wanted.Add(e);
      }
   }   
   return wanted;
}

TArray<APointBase *> ACoordinateSystemBase::FindPoint(FMathPoint inPoint, bool &found)
{
   TArray<APointBase *> wanted = TArray<APointBase*>();
   found = false;

   for(AGeometryBase *g : elements)
   {
      if(!MLD_PTR_CHECK(g)) return TArray<APointBase*>();
      if(g->type != GeometryType::point) { continue; }

      APointBase *e = ((APointBase *)g);
      if(e->point == inPoint)
      {
         found = true;
         wanted.Add(e);
      }
   }   
   return wanted;
}

TArray<ASphereBase *> ACoordinateSystemBase::FindSphere(FMathSphere inSphere, bool &found)
{
   TArray<ASphereBase *> wanted = TArray<ASphereBase*>();
   found = false;

   for(AGeometryBase *g : elements)
   {
      if(!MLD_PTR_CHECK(g)) return TArray<ASphereBase*>();
      if(g->type != GeometryType::sphere) { continue; }

      ASphereBase *e = ((ASphereBase *)g);
      if(e->sphere == inSphere)
      {
         found = true;
         wanted.Add(e);
      }
   }   
   return wanted;
}

// Remove Functions-----------------------------------------------------------------------------------------------------------------------------------

void ACoordinateSystemBase::Remove(AGeometryBase *target)
{
   if(!MLD_PTR_CHECK(target)) return;
   elements.Remove(target);
   target->Destroy();
}

// Utility Functions----------------------------------------------------------------------------------------------------------------------------------

float ACoordinateSystemBase::MaxVisibleLength()
{
   float length;
   FVector v = FVector(1, 1, 1);

   length = (v*maxCoordinate - v*maxCoordinate*(-1)).Size();

   return length;
}

bool ACoordinateSystemBase::NameNotUsed(FString inName)
{
   bool nameNotUsed = true;;

   for(AGeometryBase *g : elements)
   {
      if(!MLD_PTR_CHECK(g)) return true;
      if(g->type == GeometryType::cVector) { continue; }
      if(g->type == GeometryType::unit)    { continue; }

      if(g->GetName() == inName)
      {
         nameNotUsed = false;
         break;
      }
   }
   return nameNotUsed;
}



// Function Library Workaround -----------------------------------------------------------------------------------------------------------------------



void ACoordinateSystemBase::bp_debug_Screen(FString inString, FLinearColor color) 
{ 
   MLD_BLP(color.ToFColor(true), "%s", *inString); 
}

void ACoordinateSystemBase::SolveLinearSystem(FLinearSystem inLinearSystem)
{
   inLinearSystem.GetSolution();
}

// Autocast Function ---------------------------------------------------------------------------------------------------------------------------------

FString ACoordinateSystemBase::FNVectorToString(FNVector inNVector)          { return inNVector.ToString(); }
FString ACoordinateSystemBase::FNMatrixToString(FNMatrix inNMatrix)          { return inNMatrix.ToString(); }
FString ACoordinateSystemBase::FLinearSystemToString(FLinearSystem inSystem) { return inSystem.ToString(); }

FString ACoordinateSystemBase::FMathCircleToString(FMathCircle inCircle) { return inCircle.ToString(); }
FString ACoordinateSystemBase::FMathLineToString(FMathLine inLine)       { return inLine.ToString();   }
FString ACoordinateSystemBase::FMathPlaneToString(FMathPlane inPlane)    { return inPlane.ToString();  }
FString ACoordinateSystemBase::FMathPointToString(FMathPoint inPoint)    { return inPoint.ToString();  }
FString ACoordinateSystemBase::FMathSphereToString(FMathSphere inSphere) { return inSphere.ToString(); }

FString ACoordinateSystemBase::BPCircleToString(ACircleBase *inCircle)   { if(!MLD_PTR_CHECK(inCircle)) { return "";} return inCircle->ToString(); }
FString ACoordinateSystemBase::BPLineToString(ALineBase *inLine)         { if(!MLD_PTR_CHECK(inLine))   { return "";} return inLine->ToString();   }
FString ACoordinateSystemBase::BPPlaneToString(APlaneBase *inPlane)      { if(!MLD_PTR_CHECK(inPlane))  { return "";} return inPlane->ToString();  }                                                                                                                                               
FString ACoordinateSystemBase::BPPointToString(APointBase *inPoint)      { if(!MLD_PTR_CHECK(inPoint))  { return "";} return inPoint->ToString();  }
FString ACoordinateSystemBase::BPSphereToString(ASphereBase *inSphere)   { if(!MLD_PTR_CHECK(inSphere)) { return "";} return inSphere->ToString(); }

FString ACoordinateSystemBase::IntersectionToString(FIntersection inIntersection)
{
   FString s = "";
   switch(inIntersection.type)
   {
      case IntersectionType::circle:   s = FString::Printf(TEXT("Circle: %s"), *inIntersection.circle.ToString()); break;
      case IntersectionType::line:     s = FString::Printf(TEXT("Line: %s"), *inIntersection.line.ToString()); break;
      case IntersectionType::point:    s = FString::Printf(TEXT("Point: %s"), *inIntersection.point.ToString()); break;
      case IntersectionType::puncture: s = FString::Printf(TEXT("2 Points: %s, %s"), *inIntersection.puncture.entry.ToString(), *inIntersection.puncture.exit.ToString()); break;
      case IntersectionType::no:       s = "No Intersections"; break;
   }
   return s;
}

FString ACoordinateSystemBase::RelativePositionToString(FRelativePosition inRelativePosition) 
{ 
   FString s = "";
   switch(inRelativePosition.relation)
   {
      case Relation::different:     s = "Different"; break;
      case Relation::identical:     s = "Identical"; break;
      case Relation::inside:        s = "Inside"; break;
      case Relation::outside:       s = "Outside"; break;
      case Relation::parallel:      s = "Parallel"; break;
      case Relation::skew:          s = "Skew"; break;
      case Relation::undefined:     s = "Undefined"; break;
      case Relation::intersection:  s = "Intersection: " + IntersectionToString(inRelativePosition.intersections); break;
   }
   return s; 
}

FMathCircle ACoordinateSystemBase::CircleBPToFCircle(ACircleBase *inCircle) { if(!MLD_PTR_CHECK(inCircle)) { return FMathCircle(); } return inCircle->circle; }                                                                                                                                                         
FMathLine ACoordinateSystemBase::LineBPToFLine(ALineBase *inLine)           { if(!MLD_PTR_CHECK(inLine))   { return FMathLine();   } return inLine->line;     }                                                                                                                                                         
FMathPlane ACoordinateSystemBase::PlaneBPToFPlane(APlaneBase *inPlane)      { if(!MLD_PTR_CHECK(inPlane))  { return FMathPlane();  } return inPlane->plane;   }                                                                                                                                                         
FMathPoint ACoordinateSystemBase::PointBPToFPoint(APointBase *inPoint)      { if(!MLD_PTR_CHECK(inPoint))  { return FMathPoint();  } return inPoint->point;   }                                                                                                                                                         
FMathSphere ACoordinateSystemBase::SphereBPToFSphere(ASphereBase *inSphere) { if(!MLD_PTR_CHECK(inSphere)) { return FMathSphere(); } return inSphere->sphere; }

// Calculations --------------------------------------------------------------------------------------------------------------------------------------

float ACoordinateSystemBase::Distance(AGeometryBase *from, AGeometryBase *to)
{
   if(!(MLD_PTR_CHECK(from) && MLD_PTR_CHECK(to))) return 0.f;
   return m.GetDistance(from, to);
}

float ACoordinateSystemBase::DistanceSphereCenter(ASphereBase * from, ASphereBase * to)
{
   if(!(MLD_PTR_CHECK(from) && MLD_PTR_CHECK(to))) return 0.f;
   return m.GetDistanceSphereCenter(from, to);
}

FRelativePosition ACoordinateSystemBase::GetRelativePosition(AGeometryBase *from, AGeometryBase *to)
{
   if(!(MLD_PTR_CHECK(from) && MLD_PTR_CHECK(to))) return Relation::undefined;
   return m.GetRelativePosition(from, to);
}



// ---------------------------------------------------------------------------------------------------------------------------------------------------



// Extern Access for Actor to call EndPlay() in DebugTools
#ifdef _UE_BUILD_DEBUG_FLAG_
ACoordinateSystemBase *g_this = NULL; 
#endif


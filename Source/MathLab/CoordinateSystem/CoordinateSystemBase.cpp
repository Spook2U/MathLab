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
   showNames = true;
   showMathData = true;

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
   newUnit->Init(this, laserColor, axis->GetUpVector()*index, FString::Printf(TEXT("%d"), index));
   newUnit->OrientateToAxis(axis);
   newUnit->Update();
}

APointBase *ACoordinateSystemBase::AddPoint(LaserColors color, FMathPoint inPoint, FString inName, bool showGuides)
{  
   APointBase *point = (APointBase *)AddGeometry(pointBP);

   MLD_PTR_CHECK(point); if(!point) return nullptr;
   point->Init(this, color, inPoint, inName);
   point->ShowVectorGuides(showGuides);
   point->Update();
   return point;
}

ALineBase *ACoordinateSystemBase::AddLine(LaserColors color, FMathLine inLine, LineMode mode, FString inName, bool showGuides)
{
   if(inLine.direction.Size() == 0) { MLD_ERR("Line not created. Invalid diection. Direction of the line cannt be {0, 0, 0}."); return nullptr; }

   ALineBase *line = (ALineBase *)AddGeometry(lineBP);

   MLD_PTR_CHECK(line); if(!line) return nullptr;
   line->Init(this, color, inLine, mode, inName);
   line->ShowVectorGuides(showGuides);
   line->Update();
   return line;
}

APlaneBase *ACoordinateSystemBase::AddPlane(LaserColors color, FMathPlane inPlane, PlaneMode mode, FString inName, bool showGuides)
{
   if((inPlane.direction1.Size() == 0) || (inPlane.direction2.Size() == 0)) { MLD_ERR("Plane not created. Invalid diection. No direction of the plane shall be {0, 0, 0}."); return nullptr; }

   APlaneBase *plane = (APlaneBase *)AddGeometry(planeBP);
   MLD_PTR_CHECK(plane); if(!plane) return nullptr;
   plane->Init(this, color, inPlane, mode, inName);
   plane->ShowVectorGuides(showGuides);
   plane->Update();
   return plane;
}

ASphereBase *ACoordinateSystemBase::AddSphere(LaserColors color, FMathSphere inSphere, FString inName, bool showGuides)
{
   if(inSphere.radius <= 0) { MLD_ERR("Sphere not created. Invalid radius. Cannt create Sphere with radius <= 0."); return nullptr; }

   ASphereBase *sphere = (ASphereBase *)AddGeometry(sphereBP);
   MLD_PTR_CHECK(sphere); if(!sphere) return nullptr;
   sphere->Init(this, color, inSphere, inName);
   sphere->ShowVectorGuides(showGuides);
   sphere->Update();
   return sphere;
}

ACircleBase *ACoordinateSystemBase::AddCircle(LaserColors color, FMathCircle inCircle, FString inName, bool showGuides)
{
   if(inCircle.radius <= 0) { MLD_ERR("Circle not created. Invalid radius. Cannt create Circle with radius <= 0."); return nullptr; }

   ACircleBase *circle = (ACircleBase *)AddGeometry(circleBP);
   MLD_PTR_CHECK(circle); if(!circle) return nullptr;
   circle->Init(this, color, inCircle, inName);
   circle->ShowVectorGuides(showGuides);
   circle->Update();
   return circle;
}

AVectorStruct *ACoordinateSystemBase::AddVectorStruct(LaserColors color, FVector pointA, FVector pointB, VectorStructMode mode, FString inName)
{
   AVectorStruct *newVectorStruct = (AVectorStruct *)AddGeometry(vectorStructBP);
   MLD_PTR_CHECK(newVectorStruct); if(!newVectorStruct) return nullptr;
   newVectorStruct->Init(this, color, pointA, pointB, mode, inName);
   newVectorStruct->Update();
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

void ACoordinateSystemBase::LE_Solve(FLinearSystem inLinearEqualation)
{
   inLinearEqualation.GetSolution();
}



FString ACoordinateSystemBase::FNVectorToString(FNVector inNVector)          { return inNVector.ToString(); }
FString ACoordinateSystemBase::FNMatrixToString(FNMatrix inNMatrix)          { return inNMatrix.ToString(); }
FString ACoordinateSystemBase::FLinearSystemToString(FLinearSystem inSystem) { return inSystem.ToString(); }

FString ACoordinateSystemBase::FMathCircleToString(FMathCircle inCircle) { return inCircle.ToString(); }
FString ACoordinateSystemBase::FMathLineToString(FMathLine inLine)       { return inLine.ToString();   }
FString ACoordinateSystemBase::FMathPlaneToString(FMathPlane inPlane)    { return inPlane.ToString();  }
FString ACoordinateSystemBase::FMathPointToString(FMathPoint inPoint)    { return inPoint.ToString();  }
FString ACoordinateSystemBase::FMathSphereToString(FMathSphere inSphere) { return inSphere.ToString(); }

FString ACoordinateSystemBase::BPCircleToString(ACircleBase *inCircle)   { if(!MLD_PTR_CHECK(inCircle)) return ""; return inCircle->ToString(); }
FString ACoordinateSystemBase::BPLineToString(ALineBase *inLine)         { if(!MLD_PTR_CHECK(inLine)) return "";   return inLine->ToString();   }
FString ACoordinateSystemBase::BPPlaneToString(APlaneBase *inPlane)      { if(!MLD_PTR_CHECK(inPlane)) return "";  return inPlane->ToString();  }                                                                                                                                               
FString ACoordinateSystemBase::BPPointToString(APointBase *inPoint)      { if(!MLD_PTR_CHECK(inPoint)) return "";  return inPoint->ToString();  }
FString ACoordinateSystemBase::BPSphereToString(ASphereBase *inSphere)   { if(!MLD_PTR_CHECK(inSphere)) return ""; return inSphere->ToString(); }

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

FMathCircle ACoordinateSystemBase::CircleBPToFCircle(ACircleBase *inCircle) { if(!MLD_PTR_CHECK(inCircle)) return FMathCircle(); return inCircle->circle; }                                                                                                                                                         
FMathLine ACoordinateSystemBase::LineBPToFLine(ALineBase *inLine)           { if(!MLD_PTR_CHECK(inLine)) return FMathLine();     return inLine->line;     }                                                                                                                                                         
FMathPlane ACoordinateSystemBase::PlaneBPToFPlane(APlaneBase *inPlane)      { if(!MLD_PTR_CHECK(inPlane)) return FMathPlane();   return inPlane->plane;   }                                                                                                                                                         
FMathPoint ACoordinateSystemBase::PointBPToFPoint(APointBase *inPoint)      { if(!MLD_PTR_CHECK(inPoint)) return FMathPoint();   return inPoint->point;   }                                                                                                                                                         
FMathSphere ACoordinateSystemBase::SphereBPToFSphere(ASphereBase *inSphere) { if(!MLD_PTR_CHECK(inSphere)) return FMathSphere(); return inSphere->sphere; }



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


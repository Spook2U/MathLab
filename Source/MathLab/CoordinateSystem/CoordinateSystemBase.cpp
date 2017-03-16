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
   xAxisName = "x";
   yAxisName = "y";
   zAxisName = "z";

   glowiness = 10.f;

// Coordinate System - Units--------------------------------------------------------------------------------------------------------------------------

   showUnitLaser = true;
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
   axisTextSize = 15;

   showCVectorName = true;
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
   xAxis  = nullptr;
   yAxis  = nullptr;
   zAxis  = nullptr;
   xArrow = nullptr;
   yArrow = nullptr;
   zArrow = nullptr;
   xName  = nullptr;
   yName  = nullptr;
   zName  = nullptr;

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
void ACoordinateSystemBase::Tick( float DeltaTime ) 
{ 
   Super::Tick( DeltaTime ); 
   RotateText(xName); 
   RotateText(yName); 
   RotateText(zName); 
}



// Test Function -------------------------------------------------------------------------------------------------------------------------------------

void ACoordinateSystemBase::TestFunction()
{
}



// Coordinate System Setup ---------------------------------------------------------------------------------------------------------------------------

void ACoordinateSystemBase::SetComponents(UStaticMeshComponent *inXAxis,  UStaticMeshComponent *inYAxis,  UStaticMeshComponent *inZAxis, 
                                          UStaticMeshComponent *inXArrow, UStaticMeshComponent *inYArrow, UStaticMeshComponent *inZArrow,
                                          UTextRenderComponent *inXName,  UTextRenderComponent *inYName,  UTextRenderComponent *inZName)
{
   if(!(MLD_PTR_CHECK(inXAxis)  && MLD_PTR_CHECK(inYAxis)  && MLD_PTR_CHECK(inZAxis) &&
        MLD_PTR_CHECK(inXArrow) && MLD_PTR_CHECK(inYArrow) && MLD_PTR_CHECK(inZArrow) &&
        MLD_PTR_CHECK(inXName)  && MLD_PTR_CHECK(inYName)  && MLD_PTR_CHECK(inZName))) 
   {
      return;
   }

   xAxis  = inXAxis;
   yAxis  = inYAxis;
   zAxis  = inZAxis;
   xArrow = inXArrow;
   yArrow = inYArrow;
   zArrow = inZArrow;
   xName  = inXName;
   yName  = inYName;
   zName  = inZName;

   SetAxisName(xAxisName, yAxisName, zAxisName);
}

void ACoordinateSystemBase::ScaleAxis(float length, float diameter)
{
   if(!(MLD_PTR_CHECK(xAxis)  && MLD_PTR_CHECK(yAxis)  && MLD_PTR_CHECK(zAxis) && 
        MLD_PTR_CHECK(xArrow) && MLD_PTR_CHECK(yArrow) && MLD_PTR_CHECK(zArrow))) return;

   axisLength = length;
   axisSize   = diameter;
   if(unitCount) { convertFactor = length * 100 / unitCount; }

   FVector scaleVector = {diameter, diameter, ((2*length) + (length/unitCount))};

   xAxis->SetWorldScale3D(scaleVector);
   yAxis->SetWorldScale3D(scaleVector);
   zAxis->SetWorldScale3D(scaleVector);

   SetLaserScale(xArrow, FVector(1.f, 1.f, 1.5f) * 5*diameter);
   SetLaserScale(yArrow, FVector(1.f, 1.f, 1.5f) * (-1)*5*diameter);
   SetLaserScale(zArrow, FVector(1.f, 1.f, 1.5f) * 5*diameter);

   MoveLaser(xArrow, Direction::up, unitCount + 0.5f);
   MoveLaser(yArrow, Direction::up, (-1)*(unitCount + 0.5f));
   MoveLaser(zArrow, Direction::up, unitCount + 0.5f);

   MoveText(xName, FVector(unitCount+1, 0, 0));
   MoveText(yName, FVector(0, unitCount+1, 0));
   MoveText(zName, FVector(0, 0, unitCount+1));

   //xName->SetWorldLocation(xArrow->GetComponentLocation());
   //yName->SetWorldLocation(yArrow->GetComponentLocation());
   //zName->SetWorldLocation(zArrow->GetComponentLocation());
}

// Temp Function Library Workaround ------------------------------------------------------------------------------------------------------------------
// Copy from GeometryBase -> Woraround cause StaticLibrary not working

void ACoordinateSystemBase::SetLaserScale(UStaticMeshComponent *laser, FVector scale)
{
   if(!MLD_PTR_CHECK(laser)) return;
   laser->SetWorldScale3D(FVector((scale.X ? scale.X : laser->GetComponentScale().X), 
                                  (scale.Y ? scale.Y : laser->GetComponentScale().Y), 
                                  (scale.Z ? scale.Z : laser->GetComponentScale().Z)
   ));
}

void ACoordinateSystemBase::ScaleLine(UStaticMeshComponent *line, float length)
{
   MoveLaser(line, Direction::up, length/2);
   SetLaserScale(line, FVector(NULL, NULL, (convertFactor/100)*length));
}

void ACoordinateSystemBase::ScaleVector(UStaticMeshComponent *line, UStaticMeshComponent *arrowhead, float lenght)
{
   MoveLaser(arrowhead, Direction::up, lenght-0.075f);
   ScaleLine(line, lenght);
}

void ACoordinateSystemBase::MoveLaser(UStaticMeshComponent *laser, Direction dir, float length)
{
   if(!(MLD_PTR_CHECK(laser))) return;

   FVector moveDirection = FVector::ZeroVector;
   switch(dir)
   {
      case Direction::forward: moveDirection = laser->GetForwardVector(); break;
      case Direction::right:   moveDirection = laser->GetRightVector();break;
      case Direction::up:      moveDirection = laser->GetUpVector();break;
   }
   laser->SetWorldLocation(((convertFactor*length)*moveDirection) + GetActorLocation());
}

void ACoordinateSystemBase::MoveText(UTextRenderComponent *textRender, FVector coordinate)
{
   if(!(MLD_PTR_CHECK(textRender))) return;
   
   FVector location = coordinate;
   location *= convertFactor;
   location *= FVector(1.f, -1.f, 1.f);
   location += GetActorLocation();

   textRender->SetWorldLocation(location);
}

void ACoordinateSystemBase::RotateText(UTextRenderComponent *name)
{
   if(!MLD_PTR_CHECK(name)) return;

   FVector actorLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(),0)->GetActorLocation() + FVector(0, 0, 64);
   FVector textLocation  = name->GetComponentLocation();

   FVector norm = UKismetMathLibrary::Normal(actorLocation - textLocation);
   FVector rotatedVector = UKismetMathLibrary::GreaterGreater_VectorRotator(norm, FRotator(0, 0, 0));
   FRotator newRotation = UKismetMathLibrary::Conv_VectorToRotator(rotatedVector);
   name->SetWorldRotation(newRotation);
}



void ACoordinateSystemBase::Update()
{
   for(AGeometryBase *g : elements)
   {
      if(!MLD_PTR_CHECK(g)) return;
      g->Update();
   }
}

// Name Functions-------------------------------------------------------------------------------------------------------------------------------------

void ACoordinateSystemBase::SetAxisName(FName inXName, FName inYName, FName inZName)
{
   SetXAxisName(inXName);
   SetYAxisName(inYName);
   SetZAxisName(inZName);
}

void ACoordinateSystemBase::SetXAxisName(FName inName)
{
   NameAxis(xName, inName);
}

void ACoordinateSystemBase::SetYAxisName(FName inName)
{
   NameAxis(yName, inName);
}

void ACoordinateSystemBase::SetZAxisName(FName inName)
{
   NameAxis(zName, inName);
}

void ACoordinateSystemBase::NameAxis(UTextRenderComponent *axis, FName name)
{
   if(!MLD_PTR_CHECK(axis)) return;
   axis->SetText(FText::FromName(name));
   axis->SetWorldSize(axisTextSize);
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
   newUnit->Init(this, unitLaserColor, axis->GetUpVector()*index, FName(*FString::Printf(TEXT("%d"), index)));
   newUnit->OrientateToAxis(axis);
   newUnit->Update();
}

APointBase *ACoordinateSystemBase::AddPoint(LaserColors color, FMathPoint inPoint, FName inName, bool showGuides)
{  
   APointBase *point = (APointBase *)AddGeometry(pointBP, this);
   if(!MLD_PTR_CHECK(point)) return nullptr;
   point->Init(this, color, inPoint, inName);
   point->ShowConstructingVector(showGuides);
   point->Update();
   return point;
}

ALineBase *ACoordinateSystemBase::AddLine(LaserColors color, FMathLine inLine, LineMode mode, FName inName, bool showGuides)
{
   if(inLine.direction.Size() == 0) { MLD_ERR("Line not created. Invalid diection. Direction of the line cannt be {0, 0, 0}."); return nullptr; }

   ALineBase *line = (ALineBase *)AddGeometry(lineBP, this);
   if(!MLD_PTR_CHECK(line)) return nullptr;
   line->Init(this, color, inLine, mode, inName);
   line->ShowConstructingVector(showGuides);
   line->Update();
   return line;
}

APlaneBase *ACoordinateSystemBase::AddPlane(LaserColors color, FMathPlane inPlane, PlaneMode mode, FName inName, bool showGuides)
{
   if((inPlane.direction1.Size() == 0) || (inPlane.direction2.Size() == 0)) { MLD_ERR("Plane not created. Invalid diection. No direction of the plane shall be {0, 0, 0}."); return nullptr; }

   APlaneBase *plane = (APlaneBase *)AddGeometry(planeBP, this);
   if(!MLD_PTR_CHECK(plane)) return nullptr;
   plane->Init(this, color, inPlane, mode, inName);
   plane->ShowConstructingVector(showGuides);
   plane->Update();
   return plane;
}

ASphereBase *ACoordinateSystemBase::AddSphere(LaserColors color, FMathSphere inSphere, FName inName, bool showGuides)
{
   if(inSphere.radius <= 0) { MLD_ERR("Sphere not created. Invalid radius. Cannt create Sphere with radius <= 0."); return nullptr; }

   ASphereBase *sphere = (ASphereBase *)AddGeometry(sphereBP, this);
   if(!MLD_PTR_CHECK(sphere)) return nullptr;
   sphere->Init(this, color, inSphere, inName);
   sphere->ShowConstructingVector(showGuides);
   sphere->Update();
   return sphere;
}

ACircleBase *ACoordinateSystemBase::AddCircle(LaserColors color, FMathCircle inCircle, FName inName, bool showGuides)
{
   if(inCircle.radius <= 0) { MLD_ERR("Circle not created. Invalid radius. Cannt create Circle with radius <= 0."); return nullptr; }

   ACircleBase *circle = (ACircleBase *)AddGeometry(circleBP, this);
   if(!MLD_PTR_CHECK(circle)) return nullptr;
   circle->Init(this, color, inCircle, inName);
   circle->ShowConstructingVector(showGuides);
   circle->Update();
   return circle;
}

ACVectorBase *ACoordinateSystemBase::AddCVector(AActor *parent, LaserColors color, FVector pointA, FVector pointB, CVectorMode mode, FName inName)
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

AGeometryBase* ACoordinateSystemBase::FindGeometryWithName(FName inName, bool &found)
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

bool ACoordinateSystemBase::NameNotUsed(FName inName)
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

bool ACoordinateSystemBase::Equal_CircleCircle(const FMathCircle& A, const FMathCircle& B) {    return A == B; }
bool ACoordinateSystemBase::Equal_LineLine(const FMathLine& A, const FMathLine& B)         {    return A == B; }
bool ACoordinateSystemBase::Equal_PlanePlane(const FMathPlane& A, const FMathPlane& B)     {    return A == B; }
bool ACoordinateSystemBase::Equal_PointPoint(const FMathPoint& A, const FMathPoint& B)     {    return A == B; }
bool ACoordinateSystemBase::Equal_SphereSphere(const FMathSphere& A, const FMathSphere& B) {    return A == B; }

FMathPlane ACoordinateSystemBase::MakePlaneFromNormal(FMathPoint point, FVector normal)
{
   return FMathPlane(point, normal);
}

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


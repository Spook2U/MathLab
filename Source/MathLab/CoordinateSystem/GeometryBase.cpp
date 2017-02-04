// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "VectorStruct.h"
#include "PointBase.h"
#include "LineBase.h"
#include "PlaneBase.h"
#include "SphereBase.h"
#include "GeometryBase.h"


AGeometryBase::AGeometryBase()              
{ 
   PrimaryActorTick.bCanEverTick = false; 

   CoordinateSystem = NULL;
   LaserCompoents;
   type = GeometryType::other;

   Size = 0.075;
}

void AGeometryBase::BeginPlay() { 
   Super::BeginPlay();
}

// Pure Functions -----------------------------------------------------------------------------------

FVector AGeometryBase::CoordinateToLocation(FVector coordinate)
{
   FVector location = coordinate;

   location *= CoordinateSystem->ConvertFactor;
   location *= FVector(1.f, -1.f, 1.f);
   location += CoordinateSystem->GetActorLocation();

   return location;
}

float AGeometryBase::Distance(AGeometryBase *from, AGeometryBase *to)
{
   if(!(MLD_PTR_CHECK(from) && MLD_PTR_CHECK(to))) return 0.f;

   float result = 0.f;

   long mode = 0;
   switch(from->type)
   {
      case GeometryType::point:  mode |= 1; break;
      case GeometryType::line:   mode |= 2; break;
      case GeometryType::plane:  mode |= 4; break;
      case GeometryType::sphere: mode |= 8; break;
      default: break;
   }
   
   mode <<= 16;

   switch(to->type)
   {
      case GeometryType::point:  mode |= 1; break;
      case GeometryType::line:   mode |= 2; break;
      case GeometryType::plane:  mode |= 4; break;
      case GeometryType::sphere: mode |= 8; break;
      default: break;
   }

   switch(mode)
   {
      case 65536*1+1: result = DistancePointTo((APointBase *)   from, (APointBase *)  to);   break;
      case 65536*1+2: result = DistancePointTo((APointBase *)   from, (ALineBase *)   to);   break;
      case 65536*1+4: result = DistancePointTo((APointBase *)   from, (APlaneBase *)  to);   break;
      case 65536*1+8: result = DistancePointTo((APointBase *)   from, (ASphereBase *) to);   break;
      case 65536*2+1: result = DistancePointTo((APointBase *)   to,   (ALineBase *)   from); break;
      case 65536*2+2: result = DistanceLineTo( (ALineBase *)    from, (ALineBase *)   to);   break;
      case 65536*2+4: result = DistanceLineTo( (ALineBase *)    from, (APlaneBase *)  to);   break;
      case 65536*2+8: result = DistanceLineTo( (ALineBase *)    from, (ASphereBase *) to);   break;
      case 65536*4+1: result = DistancePointTo((APointBase *)   to,   (APlaneBase *)  from); break;
      case 65536*4+2: result = DistanceLineTo( (ALineBase *)    to,   (APlaneBase *)  from); break;
      case 65536*4+4: result = DistancePlaneTo((APlaneBase *)   from, (APlaneBase *)  to);   break;
      case 65536*4+8: result = DistancePlaneTo((APlaneBase *)   from, (ASphereBase *) to);   break;
      case 65536*8+1: result = DistancePointTo((APointBase *)   to,   (ASphereBase *) from); break;
      case 65536*8+2: result = DistanceLineTo( (ALineBase *)    to,   (ASphereBase *) from); break;
      case 65536*8+4: result = DistancePlaneTo((APlaneBase *)   to,   (ASphereBase *) from); break;
      case 65536*8+8: result = DistanceSphereTo((ASphereBase *) from, (ASphereBase *) to);   break;
      default: break;
   }

   //MLD_LOG("%d %d", g1->type, g2->type);


   //if(g1->type == 2)
   //{
   //   ALineBase *l = nullptr;
   //   l = (ALineBase *)g1;
   //   l->BuildLine();
   //}
   //else if(g1->type == 2)
   //{

   //}


   return result;
}

void AGeometryBase::RelativePosition(AGeometryBase *from, AGeometryBase *to)
{
}



void AGeometryBase::Update()
{
   UpdateRendering();
   for(AVectorStruct *g : VectorGuides)
   {
      MLD_PTR_CHECK(g); if(!g) continue;
      g->Update();
   }
}

void AGeometryBase::UpdateRendering()
{
   float bound = CoordinateSystem->AxisLength * 200;
   for(UStaticMeshComponent *laser : LaserCompoents)
   {
      MLD_PTR_CHECK(laser); if(!laser) continue;
      laser->SetVectorParameterValueOnMaterials(TEXT("Location"), CoordinateSystem->GetActorLocation());
      laser->SetVectorParameterValueOnMaterials(TEXT("Bounds"), FVector(bound, bound, bound));
   }
}



void AGeometryBase::SetPosition(FVector coordinate)
{
   SetActorLocation(CoordinateToLocation(coordinate));
}

void AGeometryBase::SetColor(LaserColors color)
{
   FLinearColor newColor;
   float glow = 0.f;

   switch(color)
   {
      case LaserColors::blue:    newColor = FLinearColor(0.05f, 0.1f,  1.f,   1.f); glow = 3.f;  break;
      case LaserColors::green:   newColor = FLinearColor(0.2f,  1.f,   0.05f, 1.f); glow = 1.f;  break;
      case LaserColors::orange:  newColor = FLinearColor(1.f,   0.4f,  0.05f, 1.f); glow = 1.2f; break;
      case LaserColors::purple:  newColor = FLinearColor(1.f,   0.05f, 1.f,   1.f); glow = 1.5f; break;
      case LaserColors::red:     newColor = FLinearColor(1.f,   0.1f,  0.05f, 1.f); glow = 2.5f; break;
      case LaserColors::white:   newColor = FLinearColor(1.f,   1.f,   1.f,   1.f); glow = 1.f;  break;
      case LaserColors::yellow:  newColor = FLinearColor(0.75f, 1.f,   0.05f, 1.f); glow = 1.f;  break;
   }
   glow *= CoordinateSystem->Glowiness;

   for(UStaticMeshComponent *laser : LaserCompoents)
   {
      MLD_PTR_CHECK(laser); if(!laser) continue;
      laser->SetVectorParameterValueOnMaterials(FName(TEXT("LaserColor")), FVector(newColor));
      laser->SetScalarParameterValueOnMaterials(FName(TEXT("Glowiness")), glow);
   }
}

// --------------------------------------------------------------------------------------------------

void AGeometryBase::ShowVectorGuides(bool show)
{
   for(AVectorStruct *g : VectorGuides)
   {
      MLD_PTR_CHECK(g); if(!g) continue;
      g->RootComponent->SetHiddenInGame(!show, true);
   }
}

void AGeometryBase::SetValuesGeometry(ACoordinateSystemBase *coordinateSystem, LaserColors color)
{
   MLD_PTR_CHECK(coordinateSystem); if(!coordinateSystem) return;
   
   this->CoordinateSystem = coordinateSystem;
   SetColor(color);
}



// Protected-----------------------------------------------------------------------------------------

void AGeometryBase::AddLaserComponent(UStaticMeshComponent *laser)
{
   LaserCompoents.Add(laser);
}

void AGeometryBase::AddVectorGuide(AVectorStruct *guide)
{
   VectorGuides.Add(guide);
}

void AGeometryBase::CreateVectorGuides(LaserColors color)
{
}

// Math Calculations---------------------------------------------------------------------------------

float AGeometryBase::DistancePointTo(APointBase *point1, APointBase *point2)
{
   if(!(MLD_PTR_CHECK(point1) && MLD_PTR_CHECK(point2))) return 0.f;

   return UKismetMathLibrary::VSize(point2->Coordinate-point1->Coordinate);
}

float AGeometryBase::DistancePointTo(APointBase *point, ALineBase *line)
{
   if(!(MLD_PTR_CHECK(point) && MLD_PTR_CHECK(line))) return 0.f;

   FVector v = point->Coordinate - line->Position;

   return UKismetMathLibrary::VSize(FVector::CrossProduct(v, line->Direction)) / UKismetMathLibrary::VSize(line->Direction);
}

float AGeometryBase::DistancePointTo(APointBase *point, APlaneBase *plane)
{
   if(!(MLD_PTR_CHECK(point) && MLD_PTR_CHECK(plane))) return 0.f;

   return 1.0f;
}

float AGeometryBase::DistancePointTo(APointBase *point, ASphereBase *sphere)
{
   if(!(MLD_PTR_CHECK(point) && MLD_PTR_CHECK(sphere))) return 0.f;

   return 2.0f;
}

float AGeometryBase::DistanceLineTo(ALineBase *line1, ALineBase *line2)
{
   if(!(MLD_PTR_CHECK(line1) && MLD_PTR_CHECK(line2))) return 0.f;

   return 3.0f;
}

float AGeometryBase::DistanceLineTo(ALineBase *line, APlaneBase *plane)
{
   if(!(MLD_PTR_CHECK(line) && MLD_PTR_CHECK(plane))) return 0.f;

   return 4.0f;
}

float AGeometryBase::DistanceLineTo(ALineBase *line, ASphereBase *sphere)
{
   if(!(MLD_PTR_CHECK(line) && MLD_PTR_CHECK(sphere))) return 0.f;

   return 5.0f;
}

float AGeometryBase::DistancePlaneTo(APlaneBase *plane1, APlaneBase *plane2)
{
   if(!(MLD_PTR_CHECK(plane1) && MLD_PTR_CHECK(plane2))) return 0.f;

   return 6.0f;
}

float AGeometryBase::DistancePlaneTo(APlaneBase *plane, ASphereBase *sphere)
{
   if(!(MLD_PTR_CHECK(plane) && MLD_PTR_CHECK(sphere))) return 0.f;

   return 7.0f;
}

float AGeometryBase::DistanceSphereTo(ASphereBase *sphere1, ASphereBase *sphere2)
{
   if(!(MLD_PTR_CHECK(sphere1) && MLD_PTR_CHECK(sphere2))) return 0.f;

   return 8.0f;
}

// Build Components----------------------------------------------------------------------------------


void AGeometryBase::SetLaserMatTransparency(UStaticMeshComponent *laser, float value)
{
   MLD_PTR_CHECK(laser); if(!laser) return;
   laser->SetScalarParameterValueOnMaterials("Transparency", value);
}

void AGeometryBase::MoveLaser(UStaticMeshComponent *laser, Direction dir, float length)
{
   MLD_PTR_CHECK(laser); if(!laser) return;
   MLD_PTR_CHECK(CoordinateSystem); if(!CoordinateSystem) return;

   FVector moveDirection = FVector::ZeroVector;
   switch(dir)
   {
      case Direction::forward: moveDirection = laser->GetForwardVector(); break;
      case Direction::right:   moveDirection = laser->GetRightVector();break;
      case Direction::up:      moveDirection = laser->GetUpVector();break;
   }
   laser->SetWorldLocation(((CoordinateSystem->ConvertFactor*length)*moveDirection) + GetActorLocation());
}

void AGeometryBase::RotateLine(FVector direction)
{
   SetActorRotation(UKismetMathLibrary::Conv_VectorToRotator(direction*FVector(1.f, -1.f, 1.f)));
}
void AGeometryBase::RotateLaserLookAt(FVector from, FVector to)
{
   SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CoordinateToLocation(from), CoordinateToLocation(to)));
}

void AGeometryBase::InitScalePoint(UStaticMeshComponent *point)
{
   SetLaserScale(point, FVector(1.f, 1.f, 1.f) * Size);
}
void AGeometryBase::InitScaleLine(UStaticMeshComponent *line)
{
   SetLaserScale(line, FVector(Size/5, Size/5, NULL));
}
void AGeometryBase::InitScaleArrowhead(UStaticMeshComponent *arrowhead)
{
   SetLaserScale(arrowhead, FVector(1.f, 1.f, 1.5f) * Size);
}

void AGeometryBase::SetLaserScale(UStaticMeshComponent *laser, FVector scale)
{
   MLD_PTR_CHECK(laser); if(!laser) return;
   laser->SetWorldScale3D(FVector((scale.X ? scale.X : laser->GetComponentScale().X), 
                                  (scale.Y ? scale.Y : laser->GetComponentScale().Y), 
                                  (scale.Z ? scale.Z : laser->GetComponentScale().Z)
                                 ));
}

void AGeometryBase::ScaleLine(UStaticMeshComponent *line, float length)
{
   MoveLaser(line, Direction::up, length/2);
   SetLaserScale(line, FVector(NULL, NULL, (CoordinateSystem->ConvertFactor/100)*length));
}
void AGeometryBase::ScaleVector(UStaticMeshComponent *line, UStaticMeshComponent *arrowhead, float lenght)
{
   MoveLaser(arrowhead, Direction::up, lenght-Size);
   ScaleLine(line, lenght);
}
void AGeometryBase::ScalePlane(UStaticMeshComponent *plane, float lenght)
{
}

void AGeometryBase::ScaleSphere(UStaticMeshComponent *sphere, float radius)
{
   SetLaserScale(sphere, FVector(1.f, 1.f, 1.f) * ((radius*2) * CoordinateSystem->ConvertFactor/100));
}






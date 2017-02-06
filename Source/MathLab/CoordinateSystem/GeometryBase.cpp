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






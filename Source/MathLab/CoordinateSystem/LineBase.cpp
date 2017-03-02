// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "LineBase.h"

#include "CoordinateSystemBase.h"
#include "VectorStruct.h"



FMathLine::FMathLine() {}
FMathLine::FMathLine(FVector inPosition, FVector inDirection) : position(inPosition), direction(inDirection) {}

FString FMathLine::ToString()
{
   return FString::Printf(TEXT("Position %s, Direction: %s"), *position.ToString(), *direction.ToString());
}

// -------------------------------------------------------------------------------------------------

ALineBase::ALineBase()
{
   line = FMathLine();
   lineMesh = nullptr;
   arrowheadMesh = nullptr;
}

void ALineBase::BeginPlay()
{
   Super::BeginPlay();
}

void ALineBase::SetComponents(TArray<UStaticMeshComponent*> components)
{
   for(UStaticMeshComponent *c : components)
   {
      MLD_PTR_CHECK(c); if(!c) continue;
      if(c->GetName().Equals("LineMesh"))      { this->lineMesh      = c; }
      if(c->GetName().Equals("ArrowheadMesh")) { this->arrowheadMesh = c; }
   }

   MLD_PTR_CHECK(lineMesh);
   MLD_PTR_CHECK(arrowheadMesh);
   if(!(lineMesh && arrowheadMesh)) return;

   InitScaleLine(lineMesh);
   InitScaleArrowhead(arrowheadMesh);
   AddLaserComponent(lineMesh);
   AddLaserComponent(arrowheadMesh);

   arrowheadMesh->SetVisibility(false);
}



void ALineBase::Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors color, FMathLine inLine, LineMode inMode)
{
   MLD_PTR_CHECK(inCoordinateSystem); if(!inCoordinateSystem) return;

   Super::Init(inCoordinateSystem, color);
   
   line = inLine;
   mode = inMode;
   type = GeometryType::line;

   switch(mode)
   {
      case LineMode::line:    CreateVectorGuides(color); break;
      case LineMode::segment: break;
      case LineMode::vector:  arrowheadMesh->SetVisibility(true); break;
   }

}



void ALineBase::Update()
{
   Super::Update();
   SetPosition(line.position);
   BuildLine();
}



void ALineBase::BuildLine()
{
   if(mode == LineMode::segment) { RotateLaserLookAt(line.position, line.direction); }
   else                          { RotateLine(line.direction); }

   if     (mode == LineMode::line)    { SetLaserScale(lineMesh, FVector(NULL, NULL, coordinateSystem->MaxVisibleLength())); }
   else if(mode == LineMode::segment) { ScaleLine(lineMesh, UKismetMathLibrary::VSize(line.direction - line.position)); }
   else                               { ScaleVector(lineMesh, arrowheadMesh, UKismetMathLibrary::VSize(line.direction)); }
}

FString ALineBase::ToString()
{
   return FString::Printf(TEXT("%s; %s"), *Super::ToString(), *line.ToString());
}

// Protected ----------------------------------------------------------------------------------------

void ALineBase::CreateVectorGuides(LaserColors color)
{
   AddVectorGuide(coordinateSystem->AddVectorStruct(color, FVector::ZeroVector, line.position, VectorStructMode::vector));
   AddVectorGuide(coordinateSystem->AddVectorStruct(color, line.position, line.direction, VectorStructMode::vector));
}

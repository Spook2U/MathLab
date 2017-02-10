// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "LineBase.h"

#include "CoordinateSystemBase.h"
#include "VectorStruct.h"



FMathLine::FMathLine() {}
FMathLine::FMathLine(FVector inPosition, FVector inDirection)
{
   Position = inPosition;
   Direction = inDirection;
}



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



void ALineBase::InitLine(ACoordinateSystemBase * coordinateSystem, LaserColors color, FMathLine inLine, LineMode mode)
{
   MLD_PTR_CHECK(coordinateSystem); if(!coordinateSystem) return;

   SetValuesGeometry(coordinateSystem, color);
   this->line = inLine;
   this->Mode = mode;
   this->type = GeometryType::line;

   switch(Mode)
   {
      case LineMode::line:    CreateVectorGuides(color); break;
      case LineMode::segment: break;
      case LineMode::vector:  arrowheadMesh->SetVisibility(true); break;
   }

}



void ALineBase::Update()
{
   Super::Update();
   SetPosition(line.Position);
   BuildLine();
}



void ALineBase::BuildLine()
{
   if(Mode == LineMode::segment) { RotateLaserLookAt(line.Position, line.Direction); }
   else                          { RotateLine(line.Direction); }

   if     (Mode == LineMode::line)    { SetLaserScale(lineMesh, FVector(NULL, NULL, CoordinateSystem->MaxVisibleLength())); }
   else if(Mode == LineMode::segment) { ScaleLine(lineMesh, UKismetMathLibrary::VSize(line.Direction - line.Position)); }
   else                               { ScaleVector(lineMesh, arrowheadMesh, UKismetMathLibrary::VSize(line.Direction)); }
}

// Protected ----------------------------------------------------------------------------------------

void ALineBase::CreateVectorGuides(LaserColors color)
{
   AddVectorGuide(CoordinateSystem->AddVectorStruct(color, FVector::ZeroVector, line.Position, VectorStructMode::vector));
   AddVectorGuide(CoordinateSystem->AddVectorStruct(color, line.Position, line.Direction, VectorStructMode::vector));
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "VectorStruct.h"



AVectorStruct::AVectorStruct()
{
   a = FVector::ZeroVector;
   b = FVector::ZeroVector;
   pointAMesh    = nullptr;
   pointBMesh    = nullptr;
   lineMesh      = nullptr;
   arrowheadMesh = nullptr;
}



void AVectorStruct::SetComponents(TArray<UStaticMeshComponent*> components)
{
   for(UStaticMeshComponent *c : components)
   {
      if(c->GetName().Equals("PointA"))    { pointAMesh    = c; }
      if(c->GetName().Equals("PointB"))    { pointBMesh    = c; }
      if(c->GetName().Equals("Line"))      { lineMesh      = c; }
      if(c->GetName().Equals("Arrowhead")) { arrowheadMesh = c; }
   }
   
   MLD_PTR_CHECK(pointAMesh);
   MLD_PTR_CHECK(pointBMesh);
   MLD_PTR_CHECK(lineMesh);
   MLD_PTR_CHECK(arrowheadMesh);
   if(!(pointAMesh && pointBMesh && lineMesh && arrowheadMesh)) return;

   InitScalePoint(pointAMesh);
   InitScalePoint(pointBMesh);
   InitScaleLine(lineMesh);
   InitScaleArrowhead(arrowheadMesh);

   AddLaserComponent(pointAMesh);
   AddLaserComponent(pointBMesh);
   AddLaserComponent(lineMesh);
   AddLaserComponent(arrowheadMesh);
}



void AVectorStruct::SetValuesVectorStruct(ACoordinateSystemBase *inCoordinateSystem, LaserColors color, FVector inA, FVector inB, VectorStructMode inMode)
{
   MLD_PTR_CHECK(inCoordinateSystem); if(!inCoordinateSystem) return;

   SetValuesGeometry(inCoordinateSystem, color);
   a = inA;
   b = inB;
   mode = inMode;
   type = GeometryType::vectorStruct;
}



void AVectorStruct::SetVisibilityPointA(bool visibility)    { pointAMesh->SetVisibility(visibility);    }
void AVectorStruct::SetVisibilityPointB(bool visibility)    { pointBMesh->SetVisibility(visibility);    }
void AVectorStruct::SetVisibilityLine(bool visibility)      { lineMesh->SetVisibility(visibility);      }
void AVectorStruct::SetVisibilityArrowhead(bool visibility) { arrowheadMesh->SetVisibility(visibility); }

void AVectorStruct::SetVisibilityForAll(bool visibility)
{
   SetVisibilityPointA(visibility);
   SetVisibilityPointB(visibility);
   SetVisibilityLine(visibility);
   SetVisibilityArrowhead(visibility);
}

void AVectorStruct::SetVisibility(bool showPointA, bool showPointB, bool showLine, bool showArrowhead)
{
   SetVisibilityPointA(showPointA);
   SetVisibilityPointB(showPointB);
   SetVisibilityLine(showLine);
   SetVisibilityArrowhead(showArrowhead);
}



void AVectorStruct::Update()
{
   Super::Update();
   SetPosition(a);
   BuildLine();
   SetPointB();
}

void AVectorStruct::BuildLine()
{
   if(mode == VectorStructMode::segment) { RotateLaserLookAt(a, b); }
   else                                  { RotateLine(b); }

   if(mode == VectorStructMode::segment) { ScaleLine(lineMesh, UKismetMathLibrary::VSize(b - a)); }
   else                                  { ScaleVector(lineMesh, arrowheadMesh, UKismetMathLibrary::VSize(b)); }
}

void AVectorStruct::SetPointB()
{
   if(mode == VectorStructMode::segment) { pointBMesh->SetWorldLocation(CoordinateToLocation(b)); }
   else                                  { pointBMesh->SetWorldLocation(CoordinateToLocation(b + a)); }
}



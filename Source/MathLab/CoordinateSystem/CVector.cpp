// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "CVector.h"



ACVector::ACVector()
{
   a = FVector::ZeroVector;
   b = FVector::ZeroVector;
   pointAMesh    = nullptr;
   pointBMesh    = nullptr;
   lineMesh      = nullptr;
   arrowheadMesh = nullptr;
}



void ACVector::SetComponents(TArray<UStaticMeshComponent*> components, UTextRenderComponent *inText)
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

   if(!MLD_PTR_CHECK(inText)) return;
   nameText = inText;
}



void ACVector::Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FVector inA, FVector inB, CVectorMode inMode, FString inName)
{
   MLD_PTR_CHECK(inCoordinateSystem); if(!inCoordinateSystem) return;

   type = GeometryType::vectorStruct;
   a = inA;
   b = inB;
   mode = inMode;
   Super::Init(inCoordinateSystem, inColor, inName);
}



void ACVector::SetVisibilityPointA(bool visibility)    { pointAMesh->SetVisibility(visibility);    }
void ACVector::SetVisibilityPointB(bool visibility)    { pointBMesh->SetVisibility(visibility);    }
void ACVector::SetVisibilityLine(bool visibility)      { lineMesh->SetVisibility(visibility);      }
void ACVector::SetVisibilityArrowhead(bool visibility) { arrowheadMesh->SetVisibility(visibility); }

void ACVector::SetVisibilityForAll(bool visibility)
{
   SetVisibilityPointA(visibility);
   SetVisibilityPointB(visibility);
   SetVisibilityLine(visibility);
   SetVisibilityArrowhead(visibility);
}

void ACVector::SetVisibility(bool showPointA, bool showPointB, bool showLine, bool showArrowhead)
{
   SetVisibilityPointA(showPointA);
   SetVisibilityPointB(showPointB);
   SetVisibilityLine(showLine);
   SetVisibilityArrowhead(showArrowhead);
}



void ACVector::Update()
{
   Super::Update();
   SetPosition(a);
   BuildLine();
   MovePointB();
}

void ACVector::BuildLine()
{
   if(mode == CVectorMode::segment) { RotateLaserLookAt(a, b); }
   else                             { RotateLine(b); }

   if(mode == CVectorMode::segment) { ScaleLine(lineMesh, UKismetMathLibrary::VSize(b - a)); }
   else                             { ScaleVector(lineMesh, arrowheadMesh, UKismetMathLibrary::VSize(b)); }
}

void ACVector::MovePointB()
{
   if(mode == CVectorMode::segment) { pointBMesh->SetWorldLocation(CoordinateToLocation(b)); }
   else                             { pointBMesh->SetWorldLocation(CoordinateToLocation(b + a)); }
}



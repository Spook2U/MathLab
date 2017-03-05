// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "CVectorBase.h"



ACVectorBase::ACVectorBase()
{
   a = FVector::ZeroVector;
   b = FVector::ZeroVector;
   pointAMesh    = nullptr;
   pointBMesh    = nullptr;
   lineMesh      = nullptr;
   arrowheadMesh = nullptr;
}



void ACVectorBase::SetComponents(TArray<UStaticMeshComponent*> components, UTextRenderComponent *inText)
{
   for(UStaticMeshComponent *c : components)
   {
      if(c->GetName().Equals("PointAMesh"))    { pointAMesh    = c; }
      if(c->GetName().Equals("PointBMesh"))    { pointBMesh    = c; }
      if(c->GetName().Equals("LineMesh"))      { lineMesh      = c; }
      if(c->GetName().Equals("ArrowheadMesh")) { arrowheadMesh = c; }
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



void ACVectorBase::Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FVector inA, FVector inB, CVectorMode inMode, FString inName)
{
   MLD_PTR_CHECK(inCoordinateSystem); if(!inCoordinateSystem) return;

   type = GeometryType::vectorStruct;
   a = inA;
   b = inB;
   mode = inMode;
   Super::Init(inCoordinateSystem, inColor, inName);
}



void ACVectorBase::SetVisibilityPointA(bool visibility)    { pointAMesh->SetVisibility(visibility);    }
void ACVectorBase::SetVisibilityPointB(bool visibility)    { pointBMesh->SetVisibility(visibility);    }
void ACVectorBase::SetVisibilityLine(bool visibility)      { lineMesh->SetVisibility(visibility);      }
void ACVectorBase::SetVisibilityArrowhead(bool visibility) { arrowheadMesh->SetVisibility(visibility); }

void ACVectorBase::SetVisibilityForAll(bool visibility)
{
   SetVisibilityPointA(visibility);
   SetVisibilityPointB(visibility);
   SetVisibilityLine(visibility);
   SetVisibilityArrowhead(visibility);
}

void ACVectorBase::SetVisibility(bool showPointA, bool showPointB, bool showLine, bool showArrowhead)
{
   SetVisibilityPointA(showPointA);
   SetVisibilityPointB(showPointB);
   SetVisibilityLine(showLine);
   SetVisibilityArrowhead(showArrowhead);
}



void ACVectorBase::Update()
{
   MLD_LOG("%s %s", *a.ToString(), *b.ToString());
   Super::Update();
   SetPosition(a);
   BuildCVector();
   MovePointB();
}

void ACVectorBase::BuildCVector()
{
   if(mode == CVectorMode::segment) { RotateLaserLookAt(a, b); }
   else                             { RotateLine(b); }

   if(mode == CVectorMode::segment) { ScaleLine(lineMesh, UKismetMathLibrary::VSize(b - a)); }
   else                             { ScaleVector(lineMesh, arrowheadMesh, UKismetMathLibrary::VSize(b)); }

   //if(mode == CVectorMode::segment) { MoveText(nameText, (a + b) / 2); }
   //else                             { MoveText(nameText,  a + b/2); }
}

void ACVectorBase::MovePointB()
{
   if(mode == CVectorMode::segment) { pointBMesh->SetWorldLocation(CoordinateToLocation(b)); }
   else                             { pointBMesh->SetWorldLocation(CoordinateToLocation(b + a)); }
}



// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "CVectorBase.h"

#include "CoordinateSystemBase.h"



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
   nameRender = inText;
}



void ACVectorBase::Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FVector inA, FVector inB, CVectorMode inMode, FString inName)
{
   MLD_PTR_CHECK(inCoordinateSystem); if(!inCoordinateSystem) return;

   type = GeometryType::cVector;
   SetCVector(inA, inB);
   mode = inMode;
   
   switch(inMode)
   {
      case CVectorMode::point:       mathDataString = FString::Printf(TEXT("(%s, %s, %s)"),               *FString::SanitizeFloat(b.X), *FString::SanitizeFloat(b.Y), *FString::SanitizeFloat(b.Z)); break;
      case CVectorMode::segment:     mathDataString = FString::Printf(TEXT("(%s, %s, %s), (%s, %s, %s)"), *FString::SanitizeFloat(a.X), *FString::SanitizeFloat(a.Y), *FString::SanitizeFloat(a.Z), 
                                                                                                          *FString::SanitizeFloat(b.X), *FString::SanitizeFloat(b.Y), *FString::SanitizeFloat(b.Z)); break;
      case CVectorMode::vector:      
      case CVectorMode::vectorPoint: mathDataString = FString::Printf(TEXT("(%s, %s, %s)"),               *FString::SanitizeFloat(b.X), *FString::SanitizeFloat(b.Y), *FString::SanitizeFloat(b.Z)); break;
      case CVectorMode::general:     
      default:                       break;
   }
   
   Super::Init(inCoordinateSystem, inColor, inName);
   InitText(inName);
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
   Super::Update();
   
   Move(a);
   BuildCVector();
   MovePointB();
}

void ACVectorBase::BuildCVector()
{
   if(mode == CVectorMode::segment) { RotateLaserLookAt(a, b); }
   else                             { RotateLine(b); }

   if(mode == CVectorMode::segment) { ScaleLine(lineMesh, UKismetMathLibrary::VSize(b - a)); }
   else                             { ScaleVector(lineMesh, arrowheadMesh, UKismetMathLibrary::VSize(b)); }

   if(mode == CVectorMode::segment) { MoveText(nameRender, (a + b) / 2); }
   else                             { MoveText(nameRender,  a + b/2); }
}

void ACVectorBase::MovePointB()
{
   if(mode == CVectorMode::segment) { pointBMesh->SetWorldLocation(CoordinateToLocation(b)); }
   else                             { pointBMesh->SetWorldLocation(CoordinateToLocation(b + a)); }
}

void ACVectorBase::SetCVector(FVector inA, FVector inB)
{
   a = inA;
   b = inB;
}

void ACVectorBase::ShowText()
{
   ShowName(coordinateSystem->showCVectorName);
   ShowMathData(coordinateSystem->showCVectorMathData);
}



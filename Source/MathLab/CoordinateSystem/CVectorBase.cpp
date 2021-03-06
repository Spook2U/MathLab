// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "CVectorBase.h"

#include "CoordinateSystemBase.h"



// Constructing Vector Class -------------------------------------------------------------------------------------------------------------------------



ACVectorBase::ACVectorBase()
{
   a = FVector::ZeroVector;
   b = FVector::ZeroVector;
   pointAMesh    = nullptr;
   pointBMesh    = nullptr;
   lineMesh      = nullptr;
   arrowheadMesh = nullptr;
}

// Constructing Vector Setup--------------------------------------------------------------------------------------------------------------------------

void ACVectorBase::Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FVector inA, FVector inB, CVectorMode inMode, FName inName)
{
   if(!MLD_PTR_CHECK(inCoordinateSystem)) return;

   type = GeometryType::cVector;
   SetCVector(inA, inB);
   mode = inMode;

   switch(inMode)
   {
      case CVectorMode::point:       nameMathData = FName(*FString::Printf(TEXT("(%s, %s, %s)"),               *FString::SanitizeFloat(b.X), *FString::SanitizeFloat(b.Y), *FString::SanitizeFloat(b.Z))); break;
      case CVectorMode::segment:     nameMathData = FName(*FString::Printf(TEXT("(%s, %s, %s), (%s, %s, %s)"), *FString::SanitizeFloat(a.X), *FString::SanitizeFloat(a.Y), *FString::SanitizeFloat(a.Z), 
                                                                                                                 *FString::SanitizeFloat(b.X), *FString::SanitizeFloat(b.Y), *FString::SanitizeFloat(b.Z))); break;
      case CVectorMode::vector:      
      case CVectorMode::vectorPoint: nameMathData = FName(*FString::Printf(TEXT("(%s, %s, %s)"),               *FString::SanitizeFloat(b.X), *FString::SanitizeFloat(b.Y), *FString::SanitizeFloat(b.Z))); break;
      case CVectorMode::general:     
      default:                       break;
   }

   Super::Init(inCoordinateSystem, inColor, inName);
   InitName(inName);
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

   if(!(MLD_PTR_CHECK(pointAMesh) && MLD_PTR_CHECK(pointBMesh) &&  MLD_PTR_CHECK(lineMesh) && MLD_PTR_CHECK(arrowheadMesh) && MLD_PTR_CHECK(inText))) return;

   InitScalePoint(pointAMesh);
   InitScalePoint(pointBMesh);
   InitScaleLine(lineMesh);
   InitScaleArrowhead(arrowheadMesh);

   AddLaserComponent(pointAMesh);
   AddLaserComponent(pointBMesh);
   AddLaserComponent(lineMesh);
   AddLaserComponent(arrowheadMesh);

   nameRender = inText;
}

// Update Functions ----------------------------------------------------------------------------------------------------------------------------------

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

// Setting Functions ---------------------------------------------------------------------------------------------------------------------------------

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

void ACVectorBase::SetCVector(FVector inA, FVector inB)
{
   a = inA;
   b = inB;
}

// Name Functions-------------------------------------------------------------------------------------------------------------------------------------

void ACVectorBase::SetDefaultNameVisibility()
{
   if(!MLD_PTR_CHECK(coordinateSystem)) return;
   SetNameVisible(coordinateSystem->showCVectorName);
   SetMathDataVisible(coordinateSystem->showCVectorMathData);
}

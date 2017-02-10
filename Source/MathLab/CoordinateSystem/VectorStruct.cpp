// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "VectorStruct.h"



AVectorStruct::AVectorStruct()
{
   A = FVector::ZeroVector;
   B = FVector::ZeroVector;
   PointA = nullptr;
   PointB = nullptr;
   Line = nullptr;
   Arrowhead = nullptr;
}



void AVectorStruct::SetComponents(TArray<UStaticMeshComponent*> components)
{
   for(UStaticMeshComponent *c : components)
   {
      if(c->GetName().Equals("PointA"))    { this->PointA    = c; }
      if(c->GetName().Equals("PointB"))    { this->PointB    = c; }
      if(c->GetName().Equals("Line"))      { this->Line      = c; }
      if(c->GetName().Equals("Arrowhead")) { this->Arrowhead = c; }
   }
   
   MLD_PTR_CHECK(PointA);
   MLD_PTR_CHECK(PointB);
   MLD_PTR_CHECK(Line);
   MLD_PTR_CHECK(Arrowhead);
   if(!(PointA && PointB && Line && Arrowhead)) return;

   InitScalePoint(PointA);
   InitScalePoint(PointB);
   InitScaleLine(Line);
   InitScaleArrowhead(Arrowhead);

   AddLaserComponent(PointA);
   AddLaserComponent(PointB);
   AddLaserComponent(Line);
   AddLaserComponent(Arrowhead);
}



void AVectorStruct::SetValuesVectorStruct(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector a, FVector b, VectorStructMode mode)
{
   SetValuesGeometry(coordinateSystem, color);
   this->A = a;
   this->B = b;
   this->Mode = mode;
   this->type = GeometryType::vectorStruct;
}



void AVectorStruct::SetVisibilityPointA(bool visibility)    { PointA->SetVisibility(visibility);    }
void AVectorStruct::SetVisibilityPointB(bool visibility)    { PointB->SetVisibility(visibility);    }
void AVectorStruct::SetVisibilityLine(bool visibility)      { Line->SetVisibility(visibility);      }
void AVectorStruct::SetVisibilityArrowhead(bool visibility) { Arrowhead->SetVisibility(visibility); }

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
   SetPosition(A);
   BuildLine();
   SetPointB();
}

void AVectorStruct::BuildLine()
{
   if(Mode == VectorStructMode::segment) { RotateLaserLookAt(A, B); }
   else                                  { RotateLine(B); }

   if(Mode == VectorStructMode::segment) { ScaleLine(Line, UKismetMathLibrary::VSize(B - A)); }
   else                                  { ScaleVector(Line, Arrowhead, UKismetMathLibrary::VSize(B)); }
}

void AVectorStruct::SetPointB()
{
   if(Mode == VectorStructMode::segment) { PointB->SetWorldLocation(CoordinateToLocation(B)); }
   else                                  { PointB->SetWorldLocation(CoordinateToLocation(B + A)); }
}



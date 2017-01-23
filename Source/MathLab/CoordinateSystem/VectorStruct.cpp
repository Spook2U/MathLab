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

   ScalePointInit(PointA);
   ScalePointInit(PointB);
   ScaleLineInit(Line);
   ScaleArrowheadInit(Arrowhead);

   PointA->SetHiddenInGame(true);
   PointB->SetHiddenInGame(true);
   Line->SetHiddenInGame(true);
   Arrowhead->SetHiddenInGame(true);
   
   AddLaserComponent(PointA);
   AddLaserComponent(PointB);
   AddLaserComponent(Line);
   AddLaserComponent(Arrowhead);
}

void AVectorStruct::SetVisibilityPointA(bool visibility)    { PointA->SetVisibility(visibility);    }
void AVectorStruct::SetVisibilityPointB(bool visibility)    { PointB->SetVisibility(visibility);    }
void AVectorStruct::SetVisibilityLine(bool visibility)      { Line->SetVisibility(visibility);      }
void AVectorStruct::SetVisibilityArrowhead(bool visibility) { Arrowhead->SetVisibility(visibility); }

void AVectorStruct::SetVisibility(bool visibility)
{
   SetVisibilityPointA(visibility);
   SetVisibilityPointB(visibility);
   SetVisibilityLine(visibility);
   SetVisibilityArrowhead(visibility);
}

void AVectorStruct::SetValuesVectorStruct(ACoordinateSystemBase * coordinateSystem, LaserColors color, FVector a, FVector b)
{
   SetValues(coordinateSystem, color);
   this->A = a;
   this->B = b;
   SetVisibility(false);
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "VectorStruct.h"

void AVectorStruct::SetComponents(TArray<UStaticMeshComponent*> components)
{
   for(UStaticMeshComponent *c : components)
   {
      if(c->GetName().Equals("PointA"))    { this->PointA  = c; }
      if(c->GetName().Equals("PointB"))    { this->PointB  = c; }
      if(c->GetName().Equals("Line"))      { this->Line = c; }
      if(c->GetName().Equals("Arrowhead")) { this->Arrowhead = c; }
   }
   
   InitScalePoint(PointA);
   InitScalePoint(PointB);
   InitScaleLine(Line);
   InitScaleArrowhead(Arrowhead);

   Arrowhead->SetHiddenInGame(true);
   
   AddLaserComponent(PointA);
   AddLaserComponent(PointB);
   AddLaserComponent(Line);
   AddLaserComponent(Arrowhead);
}

void AVectorStruct::SetVisibilityPointA(bool visibility)    { PointA->SetVisibility(visibility); }
void AVectorStruct::SetVisibilityPointB(bool visibility)    { PointB->SetVisibility(visibility); }
void AVectorStruct::SetVisibilityLine(bool visibility)      { Line->SetVisibility(visibility); }
void AVectorStruct::SetVisibilityArrowhead(bool visibility) { Arrowhead->SetVisibility(visibility); }


void AVectorStruct::SetValuesVectorStruct(ACoordinateSystemBase * coordinateSystem, LaserColors color, FVector a, FVector b)
{
   SetValues(coordinateSystem, color);
   this->A = a;
   this->B = b;
}

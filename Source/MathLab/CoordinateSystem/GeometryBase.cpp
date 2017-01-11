// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "GeometryBase.h"


AGeometryBase::AGeometryBase()              
{ 
   PrimaryActorTick.bCanEverTick = false; 

   CoordinateSystem = NULL;
   LaserCompoents;
   Guides;
   IsGuide = false;

   Size = 0.075;
}

void AGeometryBase::OnConstruction(const FTransform &Transform)
{
   Super::OnConstruction(Transform);
}

void AGeometryBase::BeginPlay() { InitGeometry(); Super::BeginPlay(); 
}

void AGeometryBase::bp_debug_Screen(FString inString, FLinearColor color) { PRINTSCN(color.ToFColor(true), "%s", *inString); }



void AGeometryBase::InitGeometry_Implementation() {}



FVector AGeometryBase::CoordinateToLocation(FVector coordinate)
{
   FVector location = coordinate;

   POINTERTEST(CoordinateSystem);
   if(CoordinateSystem)
   {
      location *= CoordinateSystem->ConvertFactor;
      location *= FVector(1.f, -1.f, 1.f);
      location += CoordinateSystem->GetActorLocation();
   }
   return location;
}



void AGeometryBase::Update()
{
   UpdateRendering();
   for(AGeometryBase *g : Guides)
   {
      g->Update();
      //ist ja eig schon durch die Update()? Testen ob's so geht
      //g->UpdateRendering 
   }
}

void AGeometryBase::UpdateRendering()
{
   POINTERTEST(CoordinateSystem);
   if(CoordinateSystem)
   {
      float bound = CoordinateSystem->AxisLength * 200;
      for(UStaticMeshComponent *laser : LaserCompoents)
      {
         POINTERTEST(laser);
         if(laser)
         {
            laser->SetVectorParameterValueOnMaterials(TEXT("Location"), CoordinateSystem->GetActorLocation());
            laser->SetVectorParameterValueOnMaterials(TEXT("Bounds"), FVector(bound, bound, bound));
         }
      }
   }
}

void AGeometryBase::SetPosition(FVector coordinate)
{
   SetActorLocation(CoordinateToLocation(coordinate));
}

void AGeometryBase::SetColor(LaserColors color)
{
   FLinearColor newColor;
   float glow = 0.f;

   switch(color)
   {
      case LaserColors::blue:    newColor = FLinearColor(0.05f, 0.1f, 1.f, 1.f); glow = 3.f;  break;
      case LaserColors::green:   newColor = FLinearColor(0.2f, 1.f, 0.05f, 1.f); glow = 1.f;  break;
      case LaserColors::orange:  newColor = FLinearColor(1.f, 0.4f, 0.05f, 1.f); glow = 1.2f; break;
      case LaserColors::purple:  newColor = FLinearColor(1.f, 0.05f, 1.f, 1.f); glow = 1.5f; break;
      case LaserColors::red:     newColor = FLinearColor(1.f, 0.1f, 0.05f, 1.f); glow = 2.5f; break;
      case LaserColors::white:   newColor = FLinearColor(0.75f, 1.f, 0.05f, 1.f); glow = 1.f;  break;
      case LaserColors::yellow:  newColor = FLinearColor(1.f, 1.f, 1.f, 1.f); glow = 1.f;  break;
   }
   POINTERTEST(CoordinateSystem);
   if(CoordinateSystem)
   {
      glow *= CoordinateSystem->Glowiness;
   }

   for(UStaticMeshComponent *laser : LaserCompoents)
   {
      POINTERTEST(laser);
      if(laser)
      {
         laser->SetVectorParameterValueOnMaterials(FName(TEXT("LaserColor")), FVector(newColor));
         laser->SetScalarParameterValueOnMaterials(FName(TEXT("Glowiness")), glow);
      }
   }
}

void AGeometryBase::ShowGuides(bool show)
{
}

void AGeometryBase::AddLaserComponent(UStaticMeshComponent *laser)
{
   LaserCompoents.Add(laser);
}



void AGeometryBase::SetValues(ACoordinateSystemBase * coordinateSystem, LaserColors color)
{
   this->CoordinateSystem = coordinateSystem;
   SetColor(color);
}

void AGeometryBase::AddGuide(AGeometryBase *guide)
{
   Guides.Add(guide);
}




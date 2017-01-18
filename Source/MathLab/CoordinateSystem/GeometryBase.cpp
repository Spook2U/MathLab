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

void AGeometryBase::BeginPlay() { 
   Super::BeginPlay();
}

void AGeometryBase::bp_debug_Screen(FString inString, FLinearColor color) { MLD_BLP(color.ToFColor(true), "%s", *inString); }

// Pure Functions -----------------------------------------------------------------------------------

FVector AGeometryBase::CoordinateToLocation(FVector coordinate)
{
   FVector location = coordinate;

   MLD_PTR_CHECK(CoordinateSystem);
   if(CoordinateSystem)
   {
      location *= CoordinateSystem->ConvertFactor;
      location *= FVector(1.f, -1.f, 1.f);
      location += CoordinateSystem->GetActorLocation();
   }
   return location;
}

// Update -------------------------------------------------------------------------------------------

void AGeometryBase::Update()
{
   UpdateRendering();
   for(AGeometryBase *g : Guides)
   {
      MLD_PTR_CHECK(g);
      if(g)
      {
         g->Update();
         //ist ja eig schon durch die Update()? Testen ob's so geht
         //g->UpdateRendering 
      }
   }
}

void AGeometryBase::UpdateRendering()
{
   MLD_PTR_CHECK(CoordinateSystem);
   if(CoordinateSystem)
   {
      float bound = CoordinateSystem->AxisLength * 200;
      for(UStaticMeshComponent *laser : LaserCompoents)
      {
         MLD_PTR_CHECK(laser);
         if(laser)
         {
            laser->SetVectorParameterValueOnMaterials(TEXT("Location"), CoordinateSystem->GetActorLocation());
            laser->SetVectorParameterValueOnMaterials(TEXT("Bounds"), FVector(bound, bound, bound));
         }
      }
   }
}

// Setup --------------------------------------------------------------------------------------------

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
      case LaserColors::blue:    newColor = FLinearColor(0.05f, 0.1f,  1.f,   1.f); glow = 3.f;  break;
      case LaserColors::green:   newColor = FLinearColor(0.2f,  1.f,   0.05f, 1.f); glow = 1.f;  break;
      case LaserColors::orange:  newColor = FLinearColor(1.f,   0.4f,  0.05f, 1.f); glow = 1.2f; break;
      case LaserColors::purple:  newColor = FLinearColor(1.f,   0.05f, 1.f,   1.f); glow = 1.5f; break;
      case LaserColors::red:     newColor = FLinearColor(1.f,   0.1f,  0.05f, 1.f); glow = 2.5f; break;
      case LaserColors::white:   newColor = FLinearColor(1.f,   1.f,   1.f,   1.f); glow = 1.f;  break;
      case LaserColors::yellow:  newColor = FLinearColor(0.75f, 1.f,   0.05f, 1.f); glow = 1.f;  break;
   }
   MLD_PTR_CHECK(CoordinateSystem);
   if(CoordinateSystem) { glow *= CoordinateSystem->Glowiness; }

   for(UStaticMeshComponent *laser : LaserCompoents)
   {
      MLD_PTR_CHECK(laser);
      if(laser)
      {
         laser->SetVectorParameterValueOnMaterials(FName(TEXT("LaserColor")), FVector(newColor));
         laser->SetScalarParameterValueOnMaterials(FName(TEXT("Glowiness")), glow);
      }
   }
}

// --------------------------------------------------------------------------------------------------

void AGeometryBase::ShowGuides(bool show)
{
   for(AGeometryBase *g : Guides)
   {
      MLD_PTR_CHECK(g);
      if(g)
      {
         g->RootComponent->SetVisibility(show, true);
      }
   }
}

void AGeometryBase::SetValues(ACoordinateSystemBase * coordinateSystem, LaserColors color)
{
   this->CoordinateSystem = coordinateSystem;
   SetColor(color);
}



// Protected ----------------------------------------------------------------------------------------

void AGeometryBase::AddLaserComponent(UStaticMeshComponent *laser)
{
   LaserCompoents.Add(laser);
}

void AGeometryBase::AddGuide(AGeometryBase *guide)
{
   Guides.Add(guide);
}

void AGeometryBase::CreateGuides(LaserColors color)
{
}




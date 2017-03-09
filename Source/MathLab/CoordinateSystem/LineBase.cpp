// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "LineBase.h"

#include "CoordinateSystemBase.h"
#include "CVectorBase.h"
#include "Lib/MathLabLibrary.h"



// Math Line Structure -------------------------------------------------------------------------------------------------------------------------------

FMathLine::FMathLine() {}
FMathLine::FMathLine(FVector inPosition, FVector inDirection) : position(inPosition), direction(inDirection) {}

FMathLine &FMathLine::operator=(const FMathLine &Other)
{
   this->position = Other.position;
   this->direction = Other.direction; 

   return *this;
}
bool FMathLine::operator==(const FMathLine &Other) const
{
   MathLabLibrary m;
   return ((m.MakeUnitVector(this->direction) == m.MakeUnitVector(Other.direction)) && m.IsPointInLine(*this, FMathPoint(Other.position)));
}
bool FMathLine::operator!=(const FMathLine &Other) const
{
   return !(this == &Other);
}

FString FMathLine::ToString()
{
   return FString::Printf(TEXT("Position %s, Direction: %s"), *position.ToString(), *direction.ToString());
}

FString FMathLine::ToStringShort()
{
   return FString::Printf(TEXT("(%s, %s, %s), (%s, %s, %s)"), *FString::SanitizeFloat(position.X),  *FString::SanitizeFloat(position.Y),  *FString::SanitizeFloat(position.Z), 
                                                              *FString::SanitizeFloat(direction.X), *FString::SanitizeFloat(direction.Y), *FString::SanitizeFloat(direction.Z));
}



// Line Class ----------------------------------------------------------------------------------------------------------------------------------------



ALineBase::ALineBase()
{
   line = FMathLine();
   lineMesh = nullptr;
   arrowheadMesh = nullptr;
}

// Unreal Events -------------------------------------------------------------------------------------------------------------------------------------

void ALineBase::BeginPlay()
{
   Super::BeginPlay();
}

// Line Setup ----------------------------------------------------------------------------------------------------------------------------------------

void ALineBase::Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FMathLine inLine, LineMode inMode, FName inName)
{
   MLD_PTR_CHECK(inCoordinateSystem); if(!inCoordinateSystem) return;

   type = GeometryType::line;
   line = inLine;
   mode = inMode;

   switch(mode)
   {
      case LineMode::line:    nameMathData = FName(*inLine.ToStringShort()); break;
      case LineMode::segment: nameMathData = FName(*inLine.ToStringShort()); break;
      case LineMode::vector:  nameMathData = FName(*FString::Printf(TEXT("(%s, %s, %s)"), *FString::SanitizeFloat(inLine.direction.X), *FString::SanitizeFloat(inLine.direction.Y), *FString::SanitizeFloat(inLine.direction.Z))); break;
   }

   Super::Init(inCoordinateSystem, inColor, inName);

   switch(mode)
   {
      case LineMode::line:    CreateCVector(inColor); break;
      case LineMode::segment: break;
      case LineMode::vector:  arrowheadMesh->SetVisibility(true); break;
   }

   InitName(inName);
}

void ALineBase::SetComponents(TArray<UStaticMeshComponent*> components, UTextRenderComponent *inText)
{
   for(UStaticMeshComponent *c : components)
   {
      MLD_PTR_CHECK(c); if(!c) continue;
      if(c->GetName().Equals("LineMesh"))      { this->lineMesh      = c; }
      if(c->GetName().Equals("ArrowheadMesh")) { this->arrowheadMesh = c; }
   }

   MLD_PTR_CHECK(lineMesh);
   MLD_PTR_CHECK(arrowheadMesh);
   if(!(lineMesh && arrowheadMesh)) return;

   InitScaleLine(lineMesh);
   InitScaleArrowhead(arrowheadMesh);
   AddLaserComponent(lineMesh);
   AddLaserComponent(arrowheadMesh);

   arrowheadMesh->SetVisibility(false);

   if(!MLD_PTR_CHECK(inText)) return;
   nameRender = inText;
}

// Update Functions ----------------------------------------------------------------------------------------------------------------------------------

void ALineBase::Update()
{
   Super::Update();
   Move(line.position);
   BuildLine();
}

void ALineBase::BuildLine()
{
   if(mode == LineMode::segment) { RotateLaserLookAt(line.position, line.direction); }
   else                          { RotateLine(line.direction); }

   if     (mode == LineMode::line)    { SetLaserScale(lineMesh, FVector(NULL, NULL, coordinateSystem->MaxVisibleLength())); }
   else if(mode == LineMode::segment) { ScaleLine(lineMesh, UKismetMathLibrary::VSize(line.direction - line.position)); }
   else                               { ScaleVector(lineMesh, arrowheadMesh, UKismetMathLibrary::VSize(line.direction)); }

   if     (mode == LineMode::segment) { MoveText(nameRender, (line.position + line.direction) / 2); }
   else if(mode == LineMode::vector)  { MoveText(nameRender,  line.position + line.direction/2); }
}

// Setting Functions ---------------------------------------------------------------------------------------------------------------------------------

ALineBase *ALineBase::SetLine(FMathLine inLine)
{
   line = inLine;
   nameMathData = FName(*inLine.ToStringShort());

   switch(mode)
   {
      case LineMode::line:    
         constVectors[0]->SetCVector(FVector::ZeroVector, line.position);
         constVectors[0]->SetCVector(line.position, line.direction);
         break;
      case LineMode::segment: 
         constVectors[0]->SetCVector(FVector::ZeroVector, line.position);
         constVectors[0]->SetCVector(FVector::ZeroVector, line.direction);
         break;
      case LineMode::vector:  
         break;
      default:
         break;
   }

   Update();

   return this;
}

// Utility Functions----------------------------------------------------------------------------------------------------------------------------------

FString ALineBase::ToString()
{
   return FString::Printf(TEXT("%s; %s"), *Super::ToString(), *line.ToString());
}

// Constructing Vector Functions ---------------------------------------------------------------------------------------------------------------------

void ALineBase::CreateCVector(LaserColors inColor)
{
   switch(mode)
   {
      case LineMode::line:    
         AddCVector(coordinateSystem->AddCVector(coordinateSystem, inColor, FVector::ZeroVector, line.position, CVectorMode::vector, "Position"));
         AddCVector(coordinateSystem->AddCVector(coordinateSystem, inColor, line.position, line.direction, CVectorMode::vector, "Direction"));
      break;
      case LineMode::segment: 
         AddCVector(coordinateSystem->AddCVector(coordinateSystem, inColor, FVector::ZeroVector, line.position,  CVectorMode::vectorPoint, "Point A"));
         AddCVector(coordinateSystem->AddCVector(coordinateSystem, inColor, FVector::ZeroVector, line.direction, CVectorMode::vectorPoint, "Point B"));
      break;
      case LineMode::vector:  
      break;
      default:
      break;
   }
}

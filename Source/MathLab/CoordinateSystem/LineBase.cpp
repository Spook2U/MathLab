// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "LineBase.h"

#include "CoordinateSystemBase.h"
#include "CVectorBase.h"



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
   return (this->position == Other.position) && (this->direction == Other.direction);
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

// -------------------------------------------------------------------------------------------------

ALineBase::ALineBase()
{
   line = FMathLine();
   lineMesh = nullptr;
   arrowheadMesh = nullptr;
}

void ALineBase::BeginPlay()
{
   Super::BeginPlay();
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



void ALineBase::Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FMathLine inLine, LineMode inMode, FString inName)
{
   MLD_PTR_CHECK(inCoordinateSystem); if(!inCoordinateSystem) return;

   type = GeometryType::line;
   line = inLine;
   mode = inMode;
   
   switch(mode)
   {
      case LineMode::line:    mathDataString = inLine.ToStringShort(); break;
      case LineMode::segment: mathDataString = inLine.ToStringShort(); break;
      case LineMode::vector:  mathDataString = FString::Printf(TEXT("(%s, %s, %s)"), *FString::SanitizeFloat(inLine.direction.X), *FString::SanitizeFloat(inLine.direction.Y), *FString::SanitizeFloat(inLine.direction.Z)); break;
   }

   Super::Init(inCoordinateSystem, inColor, inName);
   
   switch(mode)
   {
      case LineMode::line:    CreateCVector(inColor); break;
      case LineMode::segment: break;
      case LineMode::vector:  arrowheadMesh->SetVisibility(true); break;
   }

   InitText(inName);
}



void ALineBase::Update()
{
   Super::Update();
   SetPosition(line.position);
   BuildCVector();
}



void ALineBase::BuildCVector()
{
   if(mode == LineMode::segment) { RotateLaserLookAt(line.position, line.direction); }
   else                          { RotateLine(line.direction); }

   if     (mode == LineMode::line)    { SetLaserScale(lineMesh, FVector(NULL, NULL, coordinateSystem->MaxVisibleLength())); }
   else if(mode == LineMode::segment) { ScaleLine(lineMesh, UKismetMathLibrary::VSize(line.direction - line.position)); }
   else                               { ScaleVector(lineMesh, arrowheadMesh, UKismetMathLibrary::VSize(line.direction)); }

   if     (mode == LineMode::segment) { MoveText(nameRender, (line.position + line.direction) / 2); }
   else if(mode == LineMode::vector)  { MoveText(nameRender,  line.position + line.direction/2); }
}

FString ALineBase::ToString()
{
   return FString::Printf(TEXT("%s; %s"), *Super::ToString(), *line.ToString());
}

// Protected ----------------------------------------------------------------------------------------

void ALineBase::CreateCVector(LaserColors inColor)
{
   switch(mode)
   {
      case LineMode::line:    
         AddCVector(coordinateSystem->AddCVector(inColor, FVector::ZeroVector, line.position, CVectorMode::vector, "Position"));
         AddCVector(coordinateSystem->AddCVector(inColor, line.position, line.direction, CVectorMode::vector, "Direction"));
      break;
      case LineMode::segment: 
         AddCVector(coordinateSystem->AddCVector(inColor, FVector::ZeroVector, line.position,  CVectorMode::vectorPoint, "Point A"));
         AddCVector(coordinateSystem->AddCVector(inColor, FVector::ZeroVector, line.direction, CVectorMode::vectorPoint, "Point B"));
      break;
      case LineMode::vector:  
      break;
      default:
      break;
   }

}

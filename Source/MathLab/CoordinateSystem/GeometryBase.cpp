// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "GeometryBase.h"

#include "CoordinateSystemBase.h"
#include "CVectorBase.h"



AGeometryBase::AGeometryBase()              
{ 
   PrimaryActorTick.bCanEverTick = true; 

   coordinateSystem = NULL;
   laserCompoents;
   type = GeometryType::other;

   size = 0.075;
   mathDataString = "";
}

void AGeometryBase::BeginPlay() { 
   Super::BeginPlay();
}

void AGeometryBase::Tick(float DeltaTime) 
{ 
   Super::Tick(DeltaTime);
   RotateText(); 
}

// Pure Functions -----------------------------------------------------------------------------------

FVector AGeometryBase::CoordinateToLocation(FVector coordinate)
{
   FVector location = coordinate;

   location *= coordinateSystem->convertFactor;
   location *= FVector(1.f, -1.f, 1.f);
   location += coordinateSystem->GetActorLocation();

   return location;
}

// Callable Functions --------------------------------------------------------------------------------

void AGeometryBase::Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FString inName)
{
   MLD_PTR_CHECK(inCoordinateSystem); if(!inCoordinateSystem) return;

   coordinateSystem = inCoordinateSystem;
   color = inColor;
   SetColor(inColor);
}

// --------------------------------------------------------------------------------------------------

void AGeometryBase::Update()
{
   UpdateRendering();
   for(ACVectorBase *g : constVectors)
   {
      MLD_PTR_CHECK(g); if(!g) continue;
      g->Update();
   }

}

void AGeometryBase::UpdateRendering()
{
   float bound = coordinateSystem->axisLength * 200;
   for(UStaticMeshComponent *laser : laserCompoents)
   {
      MLD_PTR_CHECK(laser); if(!laser) continue;
      laser->SetVectorParameterValueOnMaterials(TEXT("Location"), coordinateSystem->GetActorLocation());
      laser->SetVectorParameterValueOnMaterials(TEXT("Bounds"), FVector(bound, bound, bound));
   }
}

void AGeometryBase::RotateText()
{
   if(!nameText) return;

   FVector actorLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(),0)->GetActorLocation() + FVector(0, 0, 64);
   FVector textLocation  = nameText->GetComponentLocation();

   FVector norm = UKismetMathLibrary::Normal(actorLocation - textLocation);
   FVector rotatedVector = UKismetMathLibrary::GreaterGreater_VectorRotator(norm, FRotator(0, 0, 0));
   FRotator newRotation = UKismetMathLibrary::Conv_VectorToRotator(rotatedVector);
   nameText->SetWorldRotation(newRotation);
}

// --------------------------------------------------------------------------------------------------

void AGeometryBase::SetColor(LaserColors inColor)
{
   FLinearColor newColor;
   float glow = 0.f;

   switch(inColor)
   {
      case LaserColors::blue:    newColor = FLinearColor(0.05f, 0.1f,  1.f,   1.f); glow = 3.f;  break;
      case LaserColors::green:   newColor = FLinearColor(0.2f,  1.f,   0.05f, 1.f); glow = 1.f;  break;
      case LaserColors::orange:  newColor = FLinearColor(1.f,   0.4f,  0.05f, 1.f); glow = 1.2f; break;
      case LaserColors::purple:  newColor = FLinearColor(1.f,   0.05f, 1.f,   1.f); glow = 1.5f; break;
      case LaserColors::red:     newColor = FLinearColor(1.f,   0.1f,  0.05f, 1.f); glow = 2.5f; break;
      case LaserColors::white:   newColor = FLinearColor(1.f,   1.f,   1.f,   1.f); glow = 1.f;  break;
      case LaserColors::yellow:  newColor = FLinearColor(0.75f, 1.f,   0.05f, 1.f); glow = 1.f;  break;
   }
   glow *= coordinateSystem->glowiness;

   for(UStaticMeshComponent *laser : laserCompoents)
   {
      MLD_PTR_CHECK(laser); if(!laser) continue;
      laser->SetVectorParameterValueOnMaterials(FName(TEXT("LaserColor")), FVector(newColor));
      laser->SetScalarParameterValueOnMaterials(FName(TEXT("Glowiness Extern")), glow);
   }
}

void AGeometryBase::SetPosition(FVector coordinate)
{
   SetActorLocation(CoordinateToLocation(coordinate));
}

void AGeometryBase::ShowName(bool show)
{
   if(type == GeometryType::unit) { showName = true; }
   else                           { showName = show; }

   BuildText(nameString);
   UpdateTextVisibility();
}

void AGeometryBase::ShowMathData(bool show)
{
   showMathData = show;
   BuildText(nameString);
   UpdateTextVisibility();
}

void AGeometryBase::ShowCVectorName(bool show)
{
   for(ACVectorBase *cv : constVectors)
   {
      cv->ShowName(show);
   }
}

void AGeometryBase::ShowCVectorMathData(bool show)
{
   for(ACVectorBase *cv : constVectors)
   {
      cv->ShowMathData(show);
   }
}


void AGeometryBase::SetName(FString inName)
{
   nameString = inName;
   nameText->SetText(BuildText(nameString));
}

void AGeometryBase::ClearName()
{
   SetName("");
}

void AGeometryBase::ShowVectorGuides(bool show)
{
   showConstruction = show;

   for(ACVectorBase *g : constVectors)
   {
      MLD_PTR_CHECK(g); if(!g) continue;
      g->RootComponent->SetHiddenInGame(!show, true);
   }
}

// --------------------------------------------------------------------------------------------------

FString AGeometryBase::ToString()
{
   return FString::Printf(TEXT(""));
}

// Protected-----------------------------------------------------------------------------------------

void AGeometryBase::CreateCVector(LaserColors inColor)
{
}

void AGeometryBase::AddLaserComponent(UStaticMeshComponent *laser)
{
   laserCompoents.Add(laser);
}

void AGeometryBase::AddCVector(ACVectorBase *guide)
{
   constVectors.Add(guide);
}

FText AGeometryBase::BuildText(FString inName)
{
   FString string = "";

   if(showName)
   {
      if(inName == "")
      {
         switch(type)
         {
            case GeometryType::circle:  string += FString::Printf(TEXT("Circle%02d"),      coordinateSystem->circleCounter++); break;
            case GeometryType::line:    string += FString::Printf(TEXT("Line%02d"),        coordinateSystem->lineCounter++); break;
            case GeometryType::plane:   string += FString::Printf(TEXT("Plane%02d"),       coordinateSystem->planeCounter++); break;
            case GeometryType::point:   string += FString::Printf(TEXT("Point%02d"),       coordinateSystem->pointCounter++); break;
            case GeometryType::sphere:  string += FString::Printf(TEXT("Sphere%02d"),      coordinateSystem->sphereCounter++); break;
            case GeometryType::unit:    string += FString::Printf(TEXT("Unit%02d"),        coordinateSystem->unitCounter++); break;
            case GeometryType::cVector: string += FString::Printf(TEXT("ConstVector%02d"), coordinateSystem->constVectorCounter++); break;
            case GeometryType::other:        
            default:                    string += FString::Printf(TEXT("Geomety%02d"),     coordinateSystem->geometryCounter++); break;
         }
      }
      else
      {
         string = inName;
      }

      if(showMathData) { string += " "; }
   }

   if(showMathData)
   {
      string += mathDataString;
   }

   return FText::FromString(string);
}

void AGeometryBase::UpdateTextVisibility()
{
   nameText->SetVisibility(showName||showMathData);
}

void AGeometryBase::InitText(FString inName)
{
   float size = 0;

   ShowText();
   SetName(inName);

   if     (type == GeometryType::unit)    { size = coordinateSystem->unitTextSize; }
   else if(type == GeometryType::cVector) { size = coordinateSystem->cVectorTextSize; }
   else                                   { size = coordinateSystem->nameTextSize; }

   nameText->SetWorldSize(size);
}

void AGeometryBase::ShowText()
{
   ShowName(coordinateSystem->showNames);
   ShowMathData(coordinateSystem->showMathData);
   ShowCVectorName(coordinateSystem->showCVectorName);
   ShowCVectorMathData(coordinateSystem->showCVectorMathData);
}



// Build Components----------------------------------------------------------------------------------

void AGeometryBase::InitScalePoint(UStaticMeshComponent *point)
{
   SetLaserScale(point, FVector(1.f, 1.f, 1.f) * size);
}
void AGeometryBase::InitScaleLine(UStaticMeshComponent *line)
{
   SetLaserScale(line, FVector(size/5, size/5, NULL));
}
void AGeometryBase::InitScaleArrowhead(UStaticMeshComponent *arrowhead)
{
   SetLaserScale(arrowhead, FVector(1.f, 1.f, 1.5f) * size);
}

void AGeometryBase::SetLaserMatTransparency(UStaticMeshComponent *laser, float value)
{
   MLD_PTR_CHECK(laser); if(!laser) return;
   laser->SetScalarParameterValueOnMaterials("Transparency", value);
}

void AGeometryBase::MoveLaser(UStaticMeshComponent *laser, Direction dir, float length)
{
   MLD_PTR_CHECK(laser); if(!laser) return;
   MLD_PTR_CHECK(coordinateSystem); if(!coordinateSystem) return;

   FVector moveDirection = FVector::ZeroVector;
   switch(dir)
   {
      case Direction::forward: moveDirection = laser->GetForwardVector(); break;
      case Direction::right:   moveDirection = laser->GetRightVector();break;
      case Direction::up:      moveDirection = laser->GetUpVector();break;
   }
   laser->SetWorldLocation(((coordinateSystem->convertFactor*length)*moveDirection) + GetActorLocation());
}

void AGeometryBase::MoveText(UTextRenderComponent *textRender, FVector coordinate)
{
   MLD_PTR_CHECK(textRender); if(!textRender) return;
   MLD_PTR_CHECK(coordinateSystem); if(!coordinateSystem) return;

   textRender->SetWorldLocation(CoordinateToLocation(coordinate));
}

void AGeometryBase::RotateLine(FVector direction)
{
   SetActorRotation(UKismetMathLibrary::Conv_VectorToRotator(direction*FVector(1.f, -1.f, 1.f)));
}
void AGeometryBase::RotateLaserLookAt(FVector from, FVector to)
{
   SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CoordinateToLocation(from), CoordinateToLocation(to)));
}

void AGeometryBase::SetLaserScale(UStaticMeshComponent *laser, FVector scale)
{
   MLD_PTR_CHECK(laser); if(!laser) return;
   laser->SetWorldScale3D(FVector((scale.X ? scale.X : laser->GetComponentScale().X), 
      (scale.Y ? scale.Y : laser->GetComponentScale().Y), 
                                  (scale.Z ? scale.Z : laser->GetComponentScale().Z)
   ));
}
void AGeometryBase::ScaleLine(UStaticMeshComponent *line, float length)
{
   MoveLaser(line, Direction::up, length/2);
   SetLaserScale(line, FVector(NULL, NULL, (coordinateSystem->convertFactor/100)*length));
}
void AGeometryBase::ScaleVector(UStaticMeshComponent *line, UStaticMeshComponent *arrowhead, float lenght)
{
   MoveLaser(arrowhead, Direction::up, lenght-size);
   ScaleLine(line, lenght);
}
void AGeometryBase::ScalePlane(UStaticMeshComponent *plane, float lenght)
{
   SetLaserScale(plane, FVector(lenght, lenght, NULL));
}
void AGeometryBase::ScaleSphere(UStaticMeshComponent *sphere, float radius)
{
   SetLaserScale(sphere, FVector(1.f, 1.f, 1.f) * ((radius*2) * coordinateSystem->convertFactor/100));
}






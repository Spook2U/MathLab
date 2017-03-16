// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "GeometryBase.h"

#include "CoordinateSystemBase.h"
#include "CVectorBase.h"



AGeometryBase::AGeometryBase()              
{ 
   PrimaryActorTick.bCanEverTick = true; 

   coordinateSystem = NULL;
   color = LaserColors::green;
   type = GeometryType::other;
   showConstruction = false;
   size = 0.075;

   nameRender = nullptr;
   name = "";
   nameMathData = "";
   showName = true;
   showMathData = true;
}

// Unreal Events -------------------------------------------------------------------------------------------------------------------------------------

void AGeometryBase::BeginPlay() { 
   Super::BeginPlay();
}

void AGeometryBase::Tick(float DeltaTime) 
{ 
   Super::Tick(DeltaTime);
   RotateText(); 
}

// Geometry Setup ------------------------------------------------------------------------------------------------------------------------------------

void AGeometryBase::Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FName inName)
{
   if(!MLD_PTR_CHECK(inCoordinateSystem)) return;

   coordinateSystem = inCoordinateSystem;
   color = inColor;
   SetColor(inColor);
}

void AGeometryBase::AddLaserComponent(UStaticMeshComponent *laser)
{
   laserCompoents.Add(laser);
}

void AGeometryBase::Update()
{
   UpdateRendering();
   for(ACVectorBase *g : constVectors)
   {
      if(!MLD_PTR_CHECK(g)) continue;
      g->Update();
   }
}

void AGeometryBase::UpdateRendering()
{
   float bound = coordinateSystem->axisLength * 200;
   for(UStaticMeshComponent *laser : laserCompoents)
   {
      if(!MLD_PTR_CHECK(laser)) continue;
      laser->SetVectorParameterValueOnMaterials(TEXT("Location"), coordinateSystem->GetActorLocation());
      laser->SetVectorParameterValueOnMaterials(TEXT("Bounds"), FVector(bound, bound, bound));
   }
}

void AGeometryBase::Move(FVector coordinate)
{
   SetActorLocation(CoordinateToLocation(coordinate));
}

// Setting Functions ---------------------------------------------------------------------------------------------------------------------------------

void AGeometryBase::ShowConstructingVector(bool show)
{
   showConstruction = show;

   for(ACVectorBase *g : constVectors)
   {
      if(!MLD_PTR_CHECK(g)) continue;
      g->RootComponent->SetHiddenInGame(!show, true);
   }
}

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
   if(!MLD_PTR_CHECK(coordinateSystem)) return;
   if(type == GeometryType::unit) { glow *= coordinateSystem->unitGlowiness; }
   else                           { glow *= coordinateSystem->glowiness; }

   for(UStaticMeshComponent *laser : laserCompoents)
   {
      if(!MLD_PTR_CHECK(laser)) continue;
      laser->SetVectorParameterValueOnMaterials(FName(TEXT("LaserColor")), FVector(newColor));
      laser->SetScalarParameterValueOnMaterials(FName(TEXT("Glowiness Extern")), glow);
   }
}

bool AGeometryBase::SetName(FName inName)
{
   bool valid = true;
   if(name != inName)
   {
      name = NameCheck(inName);
      if(inName.IsNone())
      {
         MLD_WAR("Name already in use, choose a different one.");
         valid = false;
      }
   }

   if(!MLD_PTR_CHECK(nameRender)) return false;
   nameRender->SetText(FText::FromName(BuildName(name)));

   return valid;
}

void AGeometryBase::ClearName()
{
   SetName("");
}

FName AGeometryBase::GetName()
{
   return name;
}

// Name Functions-------------------------------------------------------------------------------------------------------------------------------------

void AGeometryBase::InitName(FName inName)
{
   float textSize = 0;

   name = NameCheck(inName);
   SetName(inName);
   SetDefaultNameVisibility();

   if     (type == GeometryType::unit)    { textSize = coordinateSystem->unitTextSize; }
   else if(type == GeometryType::cVector) { textSize = coordinateSystem->cVectorTextSize; }
   else                                   { textSize = coordinateSystem->nameTextSize; }

   if(!MLD_PTR_CHECK(nameRender)) return;
   nameRender->SetWorldSize(textSize);
}

FName AGeometryBase::NameCheck(FName inName)
{
   FName newName;
   if(!MLD_PTR_CHECK(coordinateSystem)) return FName();
   if(coordinateSystem->NameNotUsed(inName)) { newName = inName; }
   else                                      { newName.SetNumber(0); }   
   return newName;
}

FName AGeometryBase::BuildName(FName inName)
{
   FString string = "";
   if(showName)
   {
      if(inName.IsNone())
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
         name = FName(*string);
      }
      else
      {
         string = inName.ToString();
      }

      if(showMathData) { string += " "; }
   }

   if(showMathData) 
   {
      string += nameMathData.ToString();
   }
   string = "   " + string;
   
   return FName(*string);
}

void AGeometryBase::SetDefaultNameVisibility()
{
   if(!MLD_PTR_CHECK(coordinateSystem)) return;
   SetNameVisible(coordinateSystem->showNames);
   SetMathDataVisible(coordinateSystem->showMathData);
   SetCVectorNameVisible(coordinateSystem->showCVectorName);
   SetCVectorMathDataVisible(coordinateSystem->showCVectorMathData);
}

void AGeometryBase::SetNameVisible(bool show)
{
   if(type == GeometryType::unit) { showName = true; }
   else                           { showName = show; }

   SetName(name);
   UpdateTextVisibility();
}

void AGeometryBase::SetMathDataVisible(bool show)
{
   showMathData = show;
   SetName(name);
   UpdateTextVisibility();
}

void AGeometryBase::SetCVectorNameVisible(bool show)
{
   for(ACVectorBase *cv : constVectors)
   {
      if(!MLD_PTR_CHECK(cv)) return;
      cv->SetNameVisible(show);
   }
}

void AGeometryBase::SetCVectorMathDataVisible(bool show)
{
   for(ACVectorBase *cv : constVectors)
   {
      if(!MLD_PTR_CHECK(cv)) return;
      cv->SetMathDataVisible(show);
   }
}

void AGeometryBase::SetupName(bool showName, bool showMathdata, bool showConstructingVectorName, bool showConstructingVectorMathData)
{
   SetNameVisible(showName);
   SetMathDataVisible(showMathdata);
   SetCVectorNameVisible(showConstructingVectorName);
   SetCVectorMathDataVisible(showConstructingVectorMathData);
}

void AGeometryBase::UpdateTextVisibility()
{
   if(!MLD_PTR_CHECK(nameRender)) return;
   nameRender->SetVisibility(showName||showMathData);
}

// Utility Functions----------------------------------------------------------------------------------------------------------------------------------

FVector AGeometryBase::CoordinateToLocation(FVector coordinate)
{
   FVector location = coordinate;

   if(!MLD_PTR_CHECK(coordinateSystem)) return FVector::ZeroVector;
   location *= coordinateSystem->convertFactor;
   location *= FVector(1.f, -1.f, 1.f);
   location += coordinateSystem->GetActorLocation();

   return location;
}

FString AGeometryBase::ToString()
{
   return FString::Printf(TEXT(""));
}

// Constructing Vector Functions ---------------------------------------------------------------------------------------------------------------------

void AGeometryBase::CreateCVector(LaserColors inColor)
{}

void AGeometryBase::AddCVector(ACVectorBase *guide)
{
   constVectors.Add(guide);
}



// StaticMeshComponent Setup -------------------------------------------------------------------------------------------------------------------------



void AGeometryBase::InitScalePoint(UStaticMeshComponent *point)
{
   SetLaserScale(point, FVector(1.f, 1.f, 1.f) * size);
}

void AGeometryBase::InitScaleLine(UStaticMeshComponent *line)
{
   SetLaserScale(line, FVector(size/3.6, size/3.6, NULL));
}

void AGeometryBase::InitScaleArrowhead(UStaticMeshComponent *arrowhead)
{
   SetLaserScale(arrowhead, FVector(1.f, 1.f, 1.5f) * size);
}


void AGeometryBase::SetLaserMatTransparency(UStaticMeshComponent *laser, float value)
{
   if(!MLD_PTR_CHECK(laser)) return;
   laser->SetScalarParameterValueOnMaterials("Transparency", value);
}


void AGeometryBase::MoveLaser(UStaticMeshComponent *laser, Direction dir, float length)
{
   if(!(MLD_PTR_CHECK(laser) && MLD_PTR_CHECK(coordinateSystem))) return;

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
   if(!(MLD_PTR_CHECK(textRender) && MLD_PTR_CHECK(coordinateSystem))) return;
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

void AGeometryBase::RotateText()
{
   if(!MLD_PTR_CHECK(nameRender)) return;

   FVector actorLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(),0)->GetActorLocation() + FVector(0, 0, 64);
   FVector textLocation  = nameRender->GetComponentLocation();

   FVector norm = UKismetMathLibrary::Normal(actorLocation - textLocation);
   FVector rotatedVector = UKismetMathLibrary::GreaterGreater_VectorRotator(norm, FRotator(0, 0, 0));
   FRotator newRotation = UKismetMathLibrary::Conv_VectorToRotator(rotatedVector);
   nameRender->SetWorldRotation(newRotation);
}


void AGeometryBase::SetLaserScale(UStaticMeshComponent *laser, FVector scale)
{
   if(!MLD_PTR_CHECK(laser)) return;
   laser->SetWorldScale3D(FVector((scale.X ? scale.X : laser->GetComponentScale().X), 
                                  (scale.Y ? scale.Y : laser->GetComponentScale().Y), 
                                  (scale.Z ? scale.Z : laser->GetComponentScale().Z)
   ));
}

void AGeometryBase::ScaleLine(UStaticMeshComponent *line, float length)
{
   if(!MLD_PTR_CHECK(coordinateSystem)) return;
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
   SetLaserScale(plane, FVector((coordinateSystem->convertFactor/100)*lenght*2, (coordinateSystem->convertFactor/100)*lenght*2, NULL));
}

void AGeometryBase::ScaleSphere(UStaticMeshComponent *sphere, float radius)
{
   if(!MLD_PTR_CHECK(coordinateSystem)) return;
   SetLaserScale(sphere, FVector(1.f, 1.f, 1.f) * ((radius*2) * coordinateSystem->convertFactor/100));
}






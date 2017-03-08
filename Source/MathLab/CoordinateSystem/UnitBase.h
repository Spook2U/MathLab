// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/PointBase.h"
#include "UnitBase.generated.h"


// Unit Class ----------------------------------------------------------------------------------------------------------------------------------------


UCLASS()
class MATHLAB_API AUnitBase : public AGeometryBase
{
	GENERATED_BODY()

public:
   AUnitBase();

   virtual void BeginPlay() override;

public:
// Member --------------------------------------------------------------------------------------------------------------------------------------------

   FVector coordinate;

private:
   UStaticMeshComponent *xAxis;
   UStaticMeshComponent *yAxis;
   UStaticMeshComponent *xLaser;
   UStaticMeshComponent *yLaser;

// Functions -----------------------------------------------------------------------------------------------------------------------------------------

public:
// Unit Setup ----------------------------------------------------------------------------------------------------------------------------------------

   void Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FVector inCoordinate, FString inName = "");

   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "math lab|unit")
   void SetComponents(TArray<UStaticMeshComponent *> components, UTextRenderComponent *inText);

   void OrientateToAxis(UStaticMeshComponent *axis);

// Update Functions ----------------------------------------------------------------------------------------------------------------------------------

   //Called, when the objects need to update the position or other values
   UFUNCTION(BlueprintCallable, Category = "math lab|unit")
   virtual void Update() override;

   //Scales the laser of the Unit to have the same length as the Axis
   UFUNCTION(BlueprintCallable, Category = "math lab|unit")
   void ScaleUnitLaser();

   void ScaleUnitLaser_AtAxis(UStaticMeshComponent *laser, UStaticMeshComponent *axis, float laserSize);

// Name Functions-------------------------------------------------------------------------------------------------------------------------------------

   // Sets the name of the object
   UFUNCTION(BlueprintCallable, Category = "math lab|unit")
   virtual void SetName(FString inName);

// Constructing Vector Functions ---------------------------------------------------------------------------------------------------------------------

   virtual void CreateCVector(LaserColors inColor) override;

};

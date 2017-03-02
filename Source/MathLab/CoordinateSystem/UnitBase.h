// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/PointBase.h"
#include "UnitBase.generated.h"



UCLASS()
class MATHLAB_API AUnitBase : public APointBase
{
	GENERATED_BODY()

public:
   AUnitBase();

   virtual void BeginPlay() override;

private:
   UStaticMeshComponent *xAxis;
   UStaticMeshComponent *yAxis;
   UStaticMeshComponent *xLaser;
   UStaticMeshComponent *yLaser;

// -------------------------------------------------------------------------------------------------

public:
   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "math lab|unit")
   void SetComponents(TArray<UStaticMeshComponent *> components);

   void Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors color, FVector coordinate);

   //Called, when the objects need to update the position or other vales
   UFUNCTION(BlueprintCallable, Category = "math lab|unit")
   virtual void Update() override;

   //Scales the laser of the Unit to have the same length as the Axis
   UFUNCTION(BlueprintCallable, Category = "math lab|unit")
   void ScaleUnitLaser();

   void OrientateToAxis(UStaticMeshComponent *axis);

protected:
   void ScaleUnitLaser_AtAxis(UStaticMeshComponent *laser, UStaticMeshComponent *axis, float laserSize);

   virtual void CreateVectorGuides(LaserColors color) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/PointBase.h"
#include "UnitBase.generated.h"

/**
 * 
 */
UCLASS()
class MATHLAB_API AUnitBase : public APointBase
{
	GENERATED_BODY()

public:
   virtual void BeginPlay() override;

private:
   UStaticMeshComponent *XAxis;
   UStaticMeshComponent *YAxis;
   UStaticMeshComponent *XLaser;
   UStaticMeshComponent *YLaser;

// -------------------------------------------------------------------------------------------------

public:
   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "unit|Init")
   void SetComponents(TArray<UStaticMeshComponent *> components);

   //Called, when the objects need to update the position or other vales
   UFUNCTION(BlueprintCallable, Category = "unit|Update")
   virtual void Update() override;

   //Scales the laser of the Unit to have the same length as the Axis
   UFUNCTION(BlueprintCallable, Category = "unit|Setup")
   void ScaleUnitLaser();

   void OrientateToAxis(UStaticMeshComponent *axis);

protected:
   void ScaleUnitLaser_AtAxis(UStaticMeshComponent *laser, UStaticMeshComponent *axis, float laserSize);

   virtual void CreateGuides(LaserColors color) override;

};

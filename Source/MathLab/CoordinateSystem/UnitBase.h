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

////////////////////////////////////////////////////////////////////////////////////////////////////
// Member Variable  ////////////////////////////////////////////////////////////////////////////////
private:
   UStaticMeshComponent *XAxis;
   UStaticMeshComponent *YAxis;
   UStaticMeshComponent *XLaser;
   UStaticMeshComponent *YLaser;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions  //////////////////////////////////////////////////////////////////////////////////////
public:
   UFUNCTION(BlueprintCallable, Category = "unit")
   void SetAxis(UStaticMeshComponent *xAxis, UStaticMeshComponent *yAxis, UStaticMeshComponent *xLaser, UStaticMeshComponent *yLaser);

   UFUNCTION(BlueprintCallable, Category = "unit")
   void ScaleUnitLaser();
   
   void OrientateToAxis(UStaticMeshComponent *axis);

   void ScaleUnitLaser_AtAxis(UStaticMeshComponent *laser, UStaticMeshComponent *axis, float laserSize);

};

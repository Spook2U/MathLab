// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/GeometryBase.h"
#include "PointBase.generated.h"

UCLASS()
class MATHLAB_API APointBase : public AGeometryBase
{
	GENERATED_BODY()

////////////////////////////////////////////////////////////////////////////////////////////////////
// Member Variable  ////////////////////////////////////////////////////////////////////////////////
public:
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "point")
   FVector Coordinate;

private:
   UStaticMeshComponent *Point;
////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions  //////////////////////////////////////////////////////////////////////////////////////
public:
   UFUNCTION(BlueprintCallable, Category = "point")
   void ConstructPoint();
   
   UFUNCTION(BlueprintCallable, Category = "point")   
   void SetComponents(UStaticMeshComponent *point);

   void SetValuesPoint(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector coordinate);

   void CreateGuidesPoint(LaserColors color);	

	
	
	
};

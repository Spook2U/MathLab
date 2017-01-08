// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/GeometryBase.h"
#include "PointBase.generated.h"

UCLASS()
class MATHLAB_API APointBase : public AGeometryBase
{
	GENERATED_BODY()

public:
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "point")
      FVector Coordinate;


public:
      void SetValuesPoint(ACoordinateSystemBase * coordinateSystem, LaserColors color, FVector coordinate);

      void CreateGuidesPoint(LaserColors color);	

	
	
	
};

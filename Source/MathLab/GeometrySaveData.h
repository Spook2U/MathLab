#pragma once

#include "CoordinateSystem/CoordinateSystemBase.h"
#include "GeometrySaveData.generated.h"

/* Used only to save and Load the geometry objects. */
USTRUCT(BlueprintType)
struct FGeometrySaveData
{
   GENERATED_BODY()

public:      
   UPROPERTY(BlueprintReadWrite, Category = "geometrySaveData")
   GeometryType type;
   UPROPERTY(BlueprintReadWrite, Category = "geometrySaveData")
   LaserColors color;
   UPROPERTY(BlueprintReadWrite, Category = "geometrySaveData")
   bool showConstruction;
   UPROPERTY(BlueprintReadWrite, Category = "geometrySaveData")
   LineMode lineMode;
   UPROPERTY(BlueprintReadWrite, Category = "geometrySaveData")
   FMathPoint point;
   UPROPERTY(BlueprintReadWrite, Category = "geometrySaveData")
   FMathLine line;
   UPROPERTY(BlueprintReadWrite, Category = "geometrySaveData")
   FMathPlane plane;
   UPROPERTY(BlueprintReadWrite, Category = "geometrySaveData")
   FMathSphere sphere;
   UPROPERTY(BlueprintReadWrite, Category = "geometrySaveData")
   FMathCircle circle;

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
//#include "Math/NVector.h"

#include "MathLabTools.generated.h"

UENUM(BlueprintType)
enum class LaserColors : uint8
{
   blue     UMETA(DisplayName = "Blue"),
   green    UMETA(DisplayName = "Green"),
   orange   UMETA(DisplayName = "Orange"),
   purple   UMETA(DisplayName = "Purple"),
   red      UMETA(DisplayName = "Red"),
   white    UMETA(DisplayName = "White"),
   yellow   UMETA(DisplayName = "Yellow")
};

UENUM(BlueprintType)
enum class LineMode : uint8
{
   line    UMETA(DisplayName = "Line"),
   segment UMETA(DisplayName = "Segment"),
   vector  UMETA(DisplayName = "Vector")
};

UENUM(BlueprintType)
enum class PlaneMode : uint8
{
   plane   UMETA(DisplayName = "Plane")
};

UENUM(BlueprintType)
enum class VectorStructMode : uint8
{
   point        UMETA(DisplayName = "Point"),
   segment      UMETA(DisplayName = "Segment"),
   vector       UMETA(DisplayName = "Vector"),
   vectorPoint  UMETA(DisplayName = "Vector pointing to Point"),
   general      UMETA(DisplayName = "General")
};

UENUM(BlueprintType)
enum class Direction : uint8
{
   forward UMETA(DisplayName = "Forward"),
   right   UMETA(DisplayName = "Right"),
   up      UMETA(DisplayName = "Up")
};


UCLASS()
class MATHLAB_API UMathLabTools : public UObject 
{
   GENERATED_BODY()

};

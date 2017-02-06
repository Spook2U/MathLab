// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"

#include "MathLabEnumLibrary.generated.h"

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

UENUM(BlueprintType)
enum class LinEqSolution : uint8
{
   one       UMETA(DisplayName = "Defined Solution"),   
   no        UMETA(DisplayName = "Not solvable"),   
   endless   UMETA(DisplayName = "Endless Solutions"),
   notSolved UMETA(DisplayName = "Not Solved")
};

UENUM(BlueprintType)
enum class RelativePosition : uint8
{
   identical     UMETA(DisplayName = "Identical"),   
   different     UMETA(DisplayName = "Different"),   
   inside        UMETA(DisplayName = "Inside"),
   outside       UMETA(DisplayName = "Outside"),
   parallel      UMETA(DisplayName = "Parallel"),   
   skew          UMETA(DisplayName = "Skew"),   
   intersection  UMETA(DisplayName = "Intersection(s)"),   
   notSolved     UMETA(DisplayName = "Not Solved")
};

enum class GeometryType : uint8
{
   point,
   unit,
   line,
   plane,
   sphere,
   vectorStruct,
   other
};

/* Defines a mathematical Point. */
USTRUCT(BlueprintType)
struct FPoint
{
   GENERATED_BODY()

public:      
   UPROPERTY(BlueprintReadWrite, Category = "point")
   FVector Coordinate;
};

/* Defines a mathematical line. */
USTRUCT(BlueprintType)
struct FLine
{
   GENERATED_BODY()

public:      
   UPROPERTY(BlueprintReadWrite, Category = "line")
   FVector Position;
   FVector Direction;
};

UCLASS()
class MATHLAB_API UMathLabEnumLibrary : public UObject 
{
   GENERATED_BODY()

};

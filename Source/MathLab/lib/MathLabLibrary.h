// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/CircleBase.h"
#include "CoordinateSystem/LineBase.h"
#include "CoordinateSystem/PlaneBase.h"
#include "CoordinateSystem/PointBase.h"
#include "CoordinateSystem/SphereBase.h"

#include "MathLabLibrary.generated.h"

class  AGeometryBase;



// To prevent misstakes with float rounding
#define EPSILON 0.0001   
#define FLOAT_EQ(x,v)     (((v - EPSILON) < x) && (x < (v + EPSILON)))  
#define FLOAT_EQ_ZERO(x) ((((0 - EPSILON) < x) && (x < (0 + EPSILON))) ? 0 : x)



/* Defines possible relations between geometry objects. */
UENUM(BlueprintType)
enum class Relation : uint8
{
   identical     UMETA(DisplayName = "Identical"),   
   different     UMETA(DisplayName = "Different"),   
   inside        UMETA(DisplayName = "Inside"),
   outside       UMETA(DisplayName = "Outside"),
   parallel      UMETA(DisplayName = "Parallel"),   
   skew          UMETA(DisplayName = "Skew"),   
   intersection  UMETA(DisplayName = "Intersection(s)"),   
   undefined     UMETA(DisplayName = "Undefined")
};

/* Defines the type of the intersection. */
UENUM(BlueprintType)
enum class IntersectionType : uint8
{
   point    UMETA(DisplayName = "Point"),   
   line     UMETA(DisplayName = "Line"),   
   circle   UMETA(DisplayName = "Circle"),   
   puncture UMETA(DisplayName = "Puncture"),   
   no       UMETA(DisplayName = "No")   
};



USTRUCT(BlueprintType)
struct FPuncture
{
   GENERATED_BODY()

public:
   UPROPERTY(BlueprintReadWrite, Category = "math lab")
   FMathPoint entry;
   UPROPERTY(BlueprintReadWrite, Category = "math lab")
   FMathPoint exit;

   FPuncture();
   FPuncture(FMathPoint inEntry, FMathPoint inExit);
};



USTRUCT(BlueprintType)
struct FIntersection
{
   GENERATED_BODY()

public:      
   UPROPERTY(BlueprintReadWrite, Category = "math lab")
   IntersectionType type;
   UPROPERTY(BlueprintReadWrite, Category = "math lab")
   FMathCircle circle;
   UPROPERTY(BlueprintReadWrite, Category = "math lab")
   FMathLine line;
   UPROPERTY(BlueprintReadWrite, Category = "math lab")
   FMathPoint point;
   UPROPERTY(BlueprintReadWrite, Category = "math lab")
   FPuncture puncture;

   FIntersection();
   FIntersection(FMathPoint inPoint);
   FIntersection(FMathLine inLine);
   FIntersection(FMathCircle inCircle);
   FIntersection(FPuncture inPuncture);
};   
   
   
/* Contains the Relative Position and possible intersections. */
USTRUCT(BlueprintType)
struct FRelativePosition
{
   GENERATED_BODY()

public:      
   UPROPERTY(BlueprintReadWrite, Category = "math lab")
   Relation relation;
   UPROPERTY(BlueprintReadWrite, Category = "math lab")
   FIntersection intersections;

   /* Default constructor (no initialization). */
   FRelativePosition();
   /* Constructor to initialise the Linear System Solution structure. */
   FRelativePosition(Relation inRelation, FIntersection inIntersections = FIntersection());
};



class MATHLAB_API MathLabLibrary
{
public:
   MathLabLibrary();
   ~MathLabLibrary();

// Geometry Calculations ---------------------------------------------------------------------------

   /* Calculates the Distance between 2 Geometry Objects. */
   float GetDistance(AGeometryBase *from, AGeometryBase *to);

   /* Determines the relatives position between 2 Geometry Objects.
      The return struct contains the relative position and depending on the relative postion an array of interception points. */
   FRelativePosition GetRelativePosition(AGeometryBase *from, AGeometryBase *with);

// Vector Calculations -----------------------------------------------------------------------------

   FVector MakeNomal(FVector a, FVector b, bool unitVector = true);
   FVector MakeUnitVector(FVector v);
   float VectorDistance(FVector a, FVector b);

   bool IsPointInLine(FMathLine line, FMathPoint point);
   bool IsPointInPlane(FMathPlane plane, FMathPoint point);
   FVector GetPointOnLine(FMathLine line, float scalar);
   FVector GetPointOnPlane(FMathPlane plane, float scalar1, float scalar2);

   float HesseNormalFormPlugIn(FMathPlane plane, FMathPoint point);
   FMathLine GetIntersectionLine(FMathPlane plane, float u);

   // Algebra Calculations ----------------------------------------------------------------------------

   float SetOfPythagorasGetA(float b, float c);
   float SetOfPythagorasGetB(float a, float c);
   float SetOfPythagorasGetC(float a, float b);


	
	
};

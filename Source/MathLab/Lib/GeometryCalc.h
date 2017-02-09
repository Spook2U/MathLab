#pragma once

#include "CoordinateSystem/GeometryBase.h"

#include "GeometryCalc.generated.h"

// Forward declarations.
class  AGeometryBase;
struct FMathPoint;
struct FMathLine;
struct FMathPlane;
struct FMathSphere;



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
   notSolved     UMETA(DisplayName = "Not Solved")
};



/* Contains the Relative Position and possible intersections. */
USTRUCT(BlueprintType)
struct FRelativePosition
{
   GENERATED_BODY()

public:      
   UPROPERTY(BlueprintReadWrite, Category = "Math Lab|Calculations")
   Relation relation;
   UPROPERTY(BlueprintReadWrite, Category = "Math Lab|Calculations")
   TArray<FVector> intersections;
};



class MATHLAB_API GeometryCalc
{
public:
    GeometryCalc();
   ~GeometryCalc();

   /* Calculates the Distance between 2 Geometry Objects.*/
   float GetDistance(AGeometryBase *from, AGeometryBase *to);
   
   /* Determines the relatives position between 2 Geometry Objects.
      The return struct contains the relative position and depending on the relative postion an array of interception points. */
   FRelativePosition GetRelativePosition(AGeometryBase *from, AGeometryBase *to);

   bool PointInLine(FMathLine line, FMathPoint point);
   bool PointInPlane(FMathPlane plane, FMathPoint point);

   float HesseNormalFormPlugIn(FMathPlane plane, FMathPoint point);

   float VectorDistance(FVector a, FVector b);

   FVector MakeNomal(FVector a, FVector b, bool unitVector = true);



protected:
   /* Helper function for the Calculation Functions. */
   long GeometryTypeToLong(GeometryType type);

   float DistanceCalc(FMathPoint  point1,  FMathPoint  point2);
   float DistanceCalc(FMathPoint  point,   FMathLine   line);
   float DistanceCalc(FMathPoint  point,   FMathPlane  plane);
   float DistanceCalc(FMathPoint  point,   FMathSphere sphere);
   float DistanceCalc(FMathLine   line,    FMathPoint  point);
   float DistanceCalc(FMathLine   line1,   FMathLine   line2);
   float DistanceCalc(FMathLine   line,    FMathPlane  plane);
   float DistanceCalc(FMathLine   line,    FMathSphere sphere);
   float DistanceCalc(FMathPlane  plane,   FMathPoint  point);
   float DistanceCalc(FMathPlane  plane,   FMathLine   line);
   float DistanceCalc(FMathPlane  plane1,  FMathPlane  plane2);
   float DistanceCalc(FMathPlane  plane,   FMathSphere sphere);
   float DistanceCalc(FMathSphere sphere,  FMathPoint  point);
   float DistanceCalc(FMathSphere sphere,  FMathLine   line);
   float DistanceCalc(FMathSphere sphere,  FMathPlane  plane);
   float DistanceCalc(FMathSphere sphere1, FMathSphere sphere2);

   FRelativePosition RelativePositionCalc(FMathPoint  point1,  FMathPoint  point2);
   FRelativePosition RelativePositionCalc(FMathPoint  point,   FMathLine   line);
   FRelativePosition RelativePositionCalc(FMathPoint  point,   FMathPlane  plane);
   FRelativePosition RelativePositionCalc(FMathPoint  point,   FMathSphere sphere);
   FRelativePosition RelativePositionCalc(FMathLine   line,    FMathPoint  point);
   FRelativePosition RelativePositionCalc(FMathLine   line1,   FMathLine   line2);
   FRelativePosition RelativePositionCalc(FMathLine   line,    FMathPlane  plane);
   FRelativePosition RelativePositionCalc(FMathLine   line,    FMathSphere sphere);
   FRelativePosition RelativePositionCalc(FMathPlane  plane,   FMathPoint  point);
   FRelativePosition RelativePositionCalc(FMathPlane  plane,   FMathLine   line);
   FRelativePosition RelativePositionCalc(FMathPlane  plane1,  FMathPlane  plane2);
   FRelativePosition RelativePositionCalc(FMathPlane  plane,   FMathSphere sphere);
   FRelativePosition RelativePositionCalc(FMathSphere sphere,  FMathPoint  point);
   FRelativePosition RelativePositionCalc(FMathSphere sphere,  FMathLine   line);
   FRelativePosition RelativePositionCalc(FMathSphere sphere,  FMathPlane  plane);
   FRelativePosition RelativePositionCalc(FMathSphere sphere1, FMathSphere sphere2);
};
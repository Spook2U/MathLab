#pragma once

#include "MathLabEnumLibrary.h"

// Forward declarations.
class AGeometryBase;
class ALineBase;
class APlaneBase;
class APointBase;
class ASphereBase;
class AUnitBase;
class AVectorStruct;



struct RelPosReturn
{
   RelativePosition type;
   TArray<FVector> vectors;
};

class MATHLAB_API GeometryCalc
{
public:
    GeometryCalc();
   ~GeometryCalc();


   
   // public Calculation Functions

   /* Calculates the Distance between 2 Geometry Objects.*/
   float GetDistance(AGeometryBase *from, AGeometryBase *to);
   
   /* Determines the relatives position between 2 Geometry Objects.
      The struct contains the relative position and depending on the relative postion an array of interception points. */
   RelPosReturn GetRelativePosition(AGeometryBase *from, AGeometryBase *to);

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

   RelPosReturn RelativePositionCalc(FMathPoint  point1,  FMathPoint  point2);
   RelPosReturn RelativePositionCalc(FMathPoint  point,   FMathLine   line);
   RelPosReturn RelativePositionCalc(FMathPoint  point,   FMathPlane  plane);
   RelPosReturn RelativePositionCalc(FMathPoint  point,   FMathSphere sphere);
   RelPosReturn RelativePositionCalc(FMathLine   line,    FMathPoint  point);
   RelPosReturn RelativePositionCalc(FMathLine   line1,   FMathLine   line2);
   RelPosReturn RelativePositionCalc(FMathLine   line,    FMathPlane  plane);
   RelPosReturn RelativePositionCalc(FMathLine   line,    FMathSphere sphere);
   RelPosReturn RelativePositionCalc(FMathPlane  plane,   FMathPoint  point);
   RelPosReturn RelativePositionCalc(FMathPlane  plane,   FMathLine   line);
   RelPosReturn RelativePositionCalc(FMathPlane  plane1,  FMathPlane  plane2);
   RelPosReturn RelativePositionCalc(FMathPlane  plane,   FMathSphere sphere);
   RelPosReturn RelativePositionCalc(FMathSphere sphere,  FMathPoint  point);
   RelPosReturn RelativePositionCalc(FMathSphere sphere,  FMathLine   line);
   RelPosReturn RelativePositionCalc(FMathSphere sphere,  FMathPlane  plane);
   RelPosReturn RelativePositionCalc(FMathSphere sphere1, FMathSphere sphere2);
};
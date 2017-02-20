#pragma once

#include "Lib/MathLabLibrary.h"



struct FCalcReturn
{
public:
   float distance;
   Relation relation;
   FIntersection intersections;

   FCalcReturn();
   FCalcReturn(float inDistance);
   FCalcReturn(Relation inRelation);
};



class MATHLAB_API Calc
{
public:
   Calc();
   ~Calc();

   //Temp Object, cause static Library not possible with Unreal v4.14 (Known Bug)
   MathLabLibrary m;

   FCalcReturn Calculate(AGeometryBase *g1, AGeometryBase *g2);

   virtual FCalcReturn CalculateWith(FMathPoint point1, FMathPoint point2);
   virtual FCalcReturn CalculateWith(FMathPoint point, FMathLine line);
   virtual FCalcReturn CalculateWith(FMathPoint point, FMathPlane plane);
   virtual FCalcReturn CalculateWith(FMathPoint point, FMathSphere sphere);

   virtual FCalcReturn CalculateWith(FMathLine line, FMathPoint point);
   virtual FCalcReturn CalculateWith(FMathLine line1, FMathLine line2);
   virtual FCalcReturn CalculateWith(FMathLine line, FMathPlane plane);
   virtual FCalcReturn CalculateWith(FMathLine line, FMathSphere sphere);

   virtual FCalcReturn CalculateWith(FMathPlane plane, FMathPoint point);
   virtual FCalcReturn CalculateWith(FMathPlane plane, FMathLine line);
   virtual FCalcReturn CalculateWith(FMathPlane plane1, FMathPlane plane2);
   virtual FCalcReturn CalculateWith(FMathPlane plane, FMathSphere sphere);

   virtual FCalcReturn CalculateWith(FMathSphere sphere, FMathPoint point);
   virtual FCalcReturn CalculateWith(FMathSphere sphere, FMathLine line);
   virtual FCalcReturn CalculateWith(FMathSphere sphere, FMathPlane plane);
   virtual FCalcReturn CalculateWith(FMathSphere sphere1, FMathSphere sphere2);
};

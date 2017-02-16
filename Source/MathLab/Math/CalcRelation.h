#pragma once

#include "Calc.h"

class MATHLAB_API CalcRelation : public Calc
{
public:
   virtual FCalcReturn CalculateWith(FMathPoint point1, FMathPoint point2) override;
   virtual FCalcReturn CalculateWith(FMathPoint point, FMathLine line) override;
   virtual FCalcReturn CalculateWith(FMathPoint point, FMathPlane plane) override;
   virtual FCalcReturn CalculateWith(FMathPoint point, FMathSphere sphere) override;

   virtual FCalcReturn CalculateWith(FMathLine line, FMathPoint point) override;
   virtual FCalcReturn CalculateWith(FMathLine line1, FMathLine line2) override;
   virtual FCalcReturn CalculateWith(FMathLine line, FMathPlane plane) override;
   virtual FCalcReturn CalculateWith(FMathLine line, FMathSphere sphere) override;

   virtual FCalcReturn CalculateWith(FMathPlane plane, FMathPoint point) override;
   virtual FCalcReturn CalculateWith(FMathPlane plane, FMathLine line) override;
   virtual FCalcReturn CalculateWith(FMathPlane plane1, FMathPlane plane2) override;
   virtual FCalcReturn CalculateWith(FMathPlane plane, FMathSphere sphere) override;

   virtual FCalcReturn CalculateWith(FMathSphere sphere, FMathPoint point) override;
   virtual FCalcReturn CalculateWith(FMathSphere sphere, FMathLine line) override;
   virtual FCalcReturn CalculateWith(FMathSphere sphere, FMathPlane plane) override;
   virtual FCalcReturn CalculateWith(FMathSphere sphere1, FMathSphere sphere2) override;
};

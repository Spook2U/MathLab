// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "MathLabLibrary.h"

#include "MathLabEnums.h"
#include "Math/LinearSystem.h"
#include "Math/CalcDistance.h"
#include "Math/CalcRelation.h"

#include "CoordinateSystem/GeometryBase.h"



FPuncture::FPuncture() {}
FPuncture::FPuncture(FMathPoint inEntry, FMathPoint inExit) : entry(inEntry), exit(inExit) {}

FIntersection::FIntersection() {}
FIntersection::FIntersection(FMathCircle inCircle) : circle(inCircle), type(IntersectionType::circle) {}
FIntersection::FIntersection(FMathLine inLine) : line(inLine), type(IntersectionType::line) {}
FIntersection::FIntersection(FMathPoint inPoint) : point(inPoint), type(IntersectionType::point) {}
FIntersection::FIntersection(FPuncture inPuncture) : puncture(inPuncture), type(IntersectionType::puncture) {}

FRelativePosition::FRelativePosition(){}
FRelativePosition::FRelativePosition(Relation inRelation, FIntersection inIntersections) : relation(inRelation), intersections(inIntersections) {}



MathLabLibrary::MathLabLibrary(){}
MathLabLibrary::~MathLabLibrary(){}



float MathLabLibrary::GetDistance(AGeometryBase *from, AGeometryBase *to)
{
   return CalcDistance().Calculate(from, to).distance;
}
FRelativePosition MathLabLibrary::GetRelativePosition(AGeometryBase *from, AGeometryBase *with)
{
   FCalcReturn calcReturn = CalcRelation().Calculate(from, with);
   return FRelativePosition(calcReturn.relation, calcReturn.intersections);
}



FVector MathLabLibrary::MakeNomal(FVector a, FVector b, bool unitVector)
{
   FVector normal = UKismetMathLibrary::Cross_VectorVector(a, b);
   if(unitVector)
   {
      MakeUnitVector(normal);
   }
   return normal;
}

FVector MathLabLibrary::MakeUnitVector(FVector v)
{
   return UKismetMathLibrary::Normal(v);
}

float MathLabLibrary::VectorDistance(FVector a, FVector b)
{
   return UKismetMathLibrary::VSize(b-a);
}



bool MathLabLibrary::IsPointInLine(FMathLine line, FMathPoint point)
{
   bool isInLine = false;

   /* point  = pos + lambda*dir
   lambda = (point - pos) / dir */
   float lambda =   (point.coordinate.X - line.position.X) / line.direction.X;
   if  ((lambda == ((point.coordinate.Y - line.position.Y) / line.direction.Y)) &&
      (lambda == ((point.coordinate.Z - line.position.Z) / line.direction.Z)))
   {
      isInLine = true;
   }

   return isInLine;
}

bool MathLabLibrary::IsPointInPlane(FMathPlane plane, FMathPoint point)
{
   bool isInPlane = false;
   FLinearSystem linearSystem = FLinearSystem(FNMatrix({FNVector({plane.direction1.X, plane.direction2.X, point.coordinate.X-plane.position.X}), 
                                                       FNVector({plane.direction1.Y, plane.direction2.Y, point.coordinate.Y-plane.position.Y}), 
   }));
   FNVector scalars;
   switch(linearSystem.GetSolution().type)
   {
      case LSSolutionType::one:       
         scalars = linearSystem.GetSolution().solution;
         if(point.coordinate.Z == (plane.position.Z + scalars.Get(0) * plane.direction1.Z + scalars.Get(1) * plane.direction2.Z))
         {
            isInPlane = true;
         }
         break;
      case LSSolutionType::endless: MLD_WAR("PointToPlane: Linear System has endless Solutions. What to do here?"); break;
      case LSSolutionType::no:      MLD_WAR("PointToPlane: Linear System has no solution."); break;
      default:                      MLD_WAR("PointToPlane: Wrong enum output"); break;
   }

   return isInPlane;
}

FVector MathLabLibrary::GetPointOnLine(FMathLine line, float scalar)
{
   return line.position + scalar * line.direction;
}

FVector MathLabLibrary::GetPointOnPlane(FMathPlane plane, float scalar1, float scalar2)
{
   return plane.position + scalar1 * plane.direction1 + scalar2 * plane.direction2;
}



float MathLabLibrary::HesseNormalFormPlugIn(FMathPlane plane, FMathPoint point)
{
   return UKismetMathLibrary::Dot_VectorVector(plane.normal, point.coordinate) - UKismetMathLibrary::Dot_VectorVector(plane.normal, plane.position);
}

FMathLine MathLabLibrary::GetIntersectionLine(FMathPlane plane, FNVector solution)
{
   FMathLine line;
   float a = plane.normal.X;
   float b = plane.normal.Y;
   float c = plane.normal.Z;
   float d = FVector::DotProduct(plane.normal, plane.position);
   
   float e = solution.Get(solution.Size()-3);
   float f = solution.Get(solution.Size()-2);
   float g = solution.Get(solution.Size()-1);
   if(f != 0) 
   { 
      FVector position  = {(d*e+b*g) / a*e, g/e, 0};
      FVector direction = {(b*f+c*e / a*e), (-1)*f/e, 1};
      line = FMathLine(position, direction); 
   }
   else { MLD_ERR("Divison through 0 not allowed."); } 
   return line;
}



float MathLabLibrary::SetOfPythagorasGetA(float b, float c)
{
   return FMath::Sqrt(c*c - b*b);
}

float MathLabLibrary::SetOfPythagorasGetB(float a, float c)
{
   return FMath::Sqrt(c*c - a*a);
}

float MathLabLibrary::SetOfPythagorasGetC(float a, float b)
{
   return FMath::Sqrt(a*a + b*b);
}


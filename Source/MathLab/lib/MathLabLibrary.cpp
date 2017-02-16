// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "MathLabLibrary.h"

#include "MathLabEnums.h"
#include "Math/LinearSystem.h"
#include "Math/CalcDistance.h"
#include "Math/CalcRelation.h"

#include "CoordinateSystem/GeometryBase.h"



FRelativePosition::FRelativePosition(){}
FRelativePosition::FRelativePosition(Relation inRelation, FIntersections inIntersections)
{
   relation = inRelation;
   intersections = inIntersections;
}



MathLabLibrary::MathLabLibrary(){}
MathLabLibrary::~MathLabLibrary(){}



float MathLabLibrary::GetDistance(AGeometryBase *from, AGeometryBase *to)
{
   return CalcDistance().Calculate(from, to).distance;
}
Relation MathLabLibrary::GetRelativePosition(AGeometryBase *from, AGeometryBase *with)
{
   return CalcRelation().Calculate(from, with).relation;
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
   float lambda =   (point.Coordinate.X - line.Position.X) / line.Direction.X;
   if  ((lambda == ((point.Coordinate.Y - line.Position.Y) / line.Direction.Y)) &&
      (lambda == ((point.Coordinate.Z - line.Position.Z) / line.Direction.Z)))
   {
      isInLine = true;
   }

   return isInLine;
}

bool MathLabLibrary::IsPointInPlane(FMathPlane plane, FMathPoint point)
{
   bool isInPlane = false;
   FLinearSystem linearSystem = FLinearSystem(FNMatrix({FNVector({plane.Direction1.X, plane.Direction2.X, point.Coordinate.X-plane.Position.X}), 
                                                       FNVector({plane.Direction1.Y, plane.Direction2.Y, point.Coordinate.Y-plane.Position.Y}), 
   }));
   FNVector scalars;
   switch(linearSystem.GetSolution().type)
   {
      case LSSolutionType::one:       
         scalars = linearSystem.GetSolution().solution;
         if(point.Coordinate.Z == (plane.Position.Z + scalars.Get(0) * plane.Direction1.Z + scalars.Get(1) * plane.Direction2.Z))
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
   return line.Position + scalar * line.Direction;
}

FVector MathLabLibrary::GetPointOnPlane(FMathPlane plane, float scalar1, float scalar2)
{
   return plane.Position + scalar1 * plane.Direction1 + scalar2 * plane.Direction2;
}



float MathLabLibrary::HesseNormalFormPlugIn(FMathPlane plane, FMathPoint point)
{
   MLD_LOG("HesseNormalFormPlugIn: Point: %s   +   Plane: %s, %s, %s   = %f", *point.Coordinate.ToString(), *plane.Position.ToString(), *plane.Direction1.ToString(), *plane.Direction2.ToString(), UKismetMathLibrary::Dot_VectorVector(plane.Normal, point.Coordinate) - UKismetMathLibrary::Dot_VectorVector(plane.Normal, plane.Position));
   return UKismetMathLibrary::Dot_VectorVector(plane.Normal, point.Coordinate) - UKismetMathLibrary::Dot_VectorVector(plane.Normal, plane.Position);
}

FMathLine MathLabLibrary::GetIntersectionLine(FMathPlane plane, float u)
{
   FMathLine line;
   if(u != 0) { line = FMathLine(plane.Position, plane.Direction1 + plane.Direction2 / u); }
   else       { MLD_ERR("u = 0; Division through 0 not allowed."); }       
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






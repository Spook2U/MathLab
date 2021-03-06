// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "MathLabLibrary.h"

#include "MathLabEnums.h"
#include "Math/LinearSystem.h"
#include "Math/CalcDistance.h"
#include "Math/CalcRelation.h"

#include "CoordinateSystem/GeometryBase.h"


// Puncture Structure --------------------------------------------------------------------------------------------------------------------------------


FPuncture::FPuncture() {}
FPuncture::FPuncture(FMathPoint inEntry, FMathPoint inExit) : entry(inEntry), exit(inExit) {}

// Intersection Structure ----------------------------------------------------------------------------------------------------------------------------

FIntersection::FIntersection() {}
FIntersection::FIntersection(FMathCircle inCircle) : circle(inCircle), type(IntersectionType::circle) {}
FIntersection::FIntersection(FMathLine inLine) : line(inLine), type(IntersectionType::line) {}
FIntersection::FIntersection(FMathPoint inPoint) : point(inPoint), type(IntersectionType::point) {}
FIntersection::FIntersection(FPuncture inPuncture) : puncture(inPuncture), type(IntersectionType::puncture) {}

// Relative Position Structure -----------------------------------------------------------------------------------------------------------------------

FRelativePosition::FRelativePosition(){}
FRelativePosition::FRelativePosition(Relation inRelation, FIntersection inIntersections) : relation(inRelation), intersections(inIntersections) {}


// MathLabLibrary Class ------------------------------------------------------------------------------------------------------------------------------


MathLabLibrary::MathLabLibrary(){}
MathLabLibrary::~MathLabLibrary(){}

// Geometry Calculations -----------------------------------------------------------------------------------------------------------------------------

float MathLabLibrary::GetDistance(AGeometryBase *from, AGeometryBase *to)
{
   if(!(MLD_PTR_CHECK(from) && MLD_PTR_CHECK(to))) return 0.f;
   return CalcDistance().Calculate(from, to).distance;
}
float MathLabLibrary::GetDistanceSphereCenter(ASphereBase *s1, ASphereBase *s2)
{
   if(!(MLD_PTR_CHECK(s1) && MLD_PTR_CHECK(s2))) return 0.f;
   return CalcDistance().CalculateWith(FMathPoint(s1->sphere.center), FMathPoint(s1->sphere.center)).distance;
}
FRelativePosition MathLabLibrary::GetRelativePosition(AGeometryBase *from, AGeometryBase *with)
{
   if(!(MLD_PTR_CHECK(from) && MLD_PTR_CHECK(with))) return FRelativePosition();
   FCalcReturn calcReturn = CalcRelation().Calculate(from, with);
   return FRelativePosition(calcReturn.relation, calcReturn.intersections);
}

// Vector Calculations -------------------------------------------------------------------------------------------------------------------------------

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
   float lambdaX = 0;
   float lambdaY = 0;
   float lambdaZ = 0;
   bool dirXZero = false;
   bool dirYZero = false;
   bool dirZZero = false;
   /* point  = pos + lambda*dir
   lambda = (point - pos) / dir */
   
   if(FLOAT_EQ(line.direction.X, 0)) { dirXZero = true; }
   else                              { lambdaX = (point.coordinate.X - line.position.X) / line.direction.X; }
   if(FLOAT_EQ(line.direction.Y, 0)) { dirYZero = true; }
   else                              { lambdaY = (point.coordinate.Y - line.position.Y) / line.direction.Y; }
   if(FLOAT_EQ(line.direction.Z, 0)) { dirZZero = true; }
   else                              { lambdaZ = (point.coordinate.Z - line.position.Z) / line.direction.Z; }

   if((dirYZero && dirZZero) || (dirXZero && dirZZero) || (dirXZero && dirYZero))
   {
      if(dirYZero && dirZZero) { isInLine = FLOAT_EQ(point.coordinate.Y, line.position.Y) && FLOAT_EQ(point.coordinate.Z, line.position.Z); }
      if(dirXZero && dirZZero) { isInLine = FLOAT_EQ(point.coordinate.X, line.position.X) && FLOAT_EQ(point.coordinate.Z, line.position.Z); }
      if(dirXZero && dirYZero) { isInLine = FLOAT_EQ(point.coordinate.X, line.position.X) && FLOAT_EQ(point.coordinate.Y, line.position.Y); }
   }
   else if(dirXZero || dirYZero || dirZZero)
   {
      if(dirXZero) { isInLine = FLOAT_EQ(lambdaY, lambdaZ) && FLOAT_EQ(point.coordinate.X, line.position.X); }
      if(dirYZero) { isInLine = FLOAT_EQ(lambdaX, lambdaZ) && FLOAT_EQ(point.coordinate.Y, line.position.Y); }
      if(dirZZero) { isInLine = FLOAT_EQ(lambdaX, lambdaY) && FLOAT_EQ(point.coordinate.Z, line.position.Z); }
   }
   else
   {
      isInLine = FLOAT_EQ(lambdaX, lambdaY) && FLOAT_EQ(lambdaX, lambdaZ);
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
         if(FLOAT_EQ(point.coordinate.Z, (plane.position.Z + scalars.Get(0) * plane.direction1.Z + scalars.Get(1) * plane.direction2.Z)))
         {
            isInPlane = true;
         }
         break;
      case LSSolutionType::endless: MLD_WAR("PointToPlane: Not possible internal error"); break;
      case LSSolutionType::no:      MLD_WAR("PointToPlane: Linear System has no solution."); break;
      default:                      MLD_WAR("PointToPlane: Wrong enum output"); break;
   }

   return isInPlane;
}

FVector MathLabLibrary::GetPointOnLine(FMathLine line, float scalar, bool useUnitDirection)
{
   FVector point;
   if(useUnitDirection) point = line.position + scalar * MakeUnitVector(line.direction);
   else                 point = line.position + scalar * line.direction;
   return point;
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

// Algebra Calculations ------------------------------------------------------------------------------------------------------------------------------

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


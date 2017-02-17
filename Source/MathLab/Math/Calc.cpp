#include "MathLab.h"
#include "Calc.h"

#include "CoordinateSystem/GeometryBase.h"



FCalcReturn::FCalcReturn(){}
FCalcReturn::FCalcReturn(float inDistance)    : distance(inDistance) {}
FCalcReturn::FCalcReturn(Relation inRelation) : relation(inRelation) {}



Calc::Calc(){}
Calc::~Calc(){}



FCalcReturn Calc::Calculate(AGeometryBase *g1, AGeometryBase *g2)
{
   if(!(MLD_PTR_CHECK(g1) && MLD_PTR_CHECK(g2))) return FCalcReturn();

   FCalcReturn ret;

   switch(g1->type)
   {
      case GeometryType::point:  
         switch(g2->type)
         {
            case GeometryType::point:  ret = CalculateWith(((APointBase *) g1)->point, ((APointBase *)  g2)->point);  break;
            case GeometryType::line:   ret = CalculateWith(((APointBase *) g1)->point, ((ALineBase *)   g2)->line);   break;
            case GeometryType::plane:  ret = CalculateWith(((APointBase *) g1)->point, ((APlaneBase *)  g2)->plane);  break;
            case GeometryType::sphere: ret = CalculateWith(((APointBase *) g1)->point, ((ASphereBase *) g2)->sphere); break;
         }
      break;
      case GeometryType::line:   
         switch(g2->type)
         {
            case GeometryType::point:  ret = CalculateWith(((ALineBase *) g1)->line, ((APointBase *)  g2)->point);  break;
            case GeometryType::line:   ret = CalculateWith(((ALineBase *) g1)->line, ((ALineBase *)   g2)->line);   break;
            case GeometryType::plane:  ret = CalculateWith(((ALineBase *) g1)->line, ((APlaneBase *)  g2)->plane);  break;
            case GeometryType::sphere: ret = CalculateWith(((ALineBase *) g1)->line, ((ASphereBase *) g2)->sphere); break;
         }
      break;
      case GeometryType::plane:  
         switch(g2->type)
         {
            case GeometryType::point:  ret = CalculateWith(((APlaneBase *) g1)->plane, ((APointBase *)  g2)->point);  break;
            case GeometryType::line:   ret = CalculateWith(((APlaneBase *) g1)->plane, ((ALineBase *)   g2)->line);   break;
            case GeometryType::plane:  ret = CalculateWith(((APlaneBase *) g1)->plane, ((APlaneBase *)  g2)->plane);  break;
            case GeometryType::sphere: ret = CalculateWith(((APlaneBase *) g1)->plane, ((ASphereBase *) g2)->sphere); break;
         }
      break;
      case GeometryType::sphere: 
         switch(g2->type)
         {
            case GeometryType::point:  ret = CalculateWith(((ASphereBase *) g1)->sphere, ((APointBase *)  g2)->point);  break;
            case GeometryType::line:   ret = CalculateWith(((ASphereBase *) g1)->sphere, ((ALineBase *)   g2)->line);   break;
            case GeometryType::plane:  ret = CalculateWith(((ASphereBase *) g1)->sphere, ((APlaneBase *)  g2)->plane);  break;
            case GeometryType::sphere: ret = CalculateWith(((ASphereBase *) g1)->sphere, ((ASphereBase *) g2)->sphere); break;
         }
      break;
   }

   return ret;
}



FCalcReturn Calc::CalculateWith(FMathPoint  point1,  FMathPoint point2)   { return FCalcReturn(); }
FCalcReturn Calc::CalculateWith(FMathPoint  point,   FMathLine line)      { return FCalcReturn(); }
FCalcReturn Calc::CalculateWith(FMathPoint  point,   FMathPlane plane)    { return FCalcReturn(); }
FCalcReturn Calc::CalculateWith(FMathPoint  point,   FMathSphere sphere)  { return FCalcReturn(); }

FCalcReturn Calc::CalculateWith(FMathLine   line,    FMathPoint point)    { return FCalcReturn(); }
FCalcReturn Calc::CalculateWith(FMathLine   line1,   FMathLine line2)     { return FCalcReturn(); }
FCalcReturn Calc::CalculateWith(FMathLine   line,    FMathPlane plane)    { return FCalcReturn(); }
FCalcReturn Calc::CalculateWith(FMathLine   line,    FMathSphere sphere)  { return FCalcReturn(); }

FCalcReturn Calc::CalculateWith(FMathPlane  plane,   FMathPoint point)    { return FCalcReturn(); }
FCalcReturn Calc::CalculateWith(FMathPlane  plane,   FMathLine line)      { return FCalcReturn(); }
FCalcReturn Calc::CalculateWith(FMathPlane  plane1,  FMathPlane plane2)   { return FCalcReturn(); }
FCalcReturn Calc::CalculateWith(FMathPlane  plane,   FMathSphere sphere)  { return FCalcReturn(); }

FCalcReturn Calc::CalculateWith(FMathSphere sphere,  FMathPoint point)    { return FCalcReturn(); }
FCalcReturn Calc::CalculateWith(FMathSphere sphere,  FMathLine line)      { return FCalcReturn(); } 
FCalcReturn Calc::CalculateWith(FMathSphere sphere,  FMathPlane plane)    { return FCalcReturn(); }
FCalcReturn Calc::CalculateWith(FMathSphere sphere1, FMathSphere sphere2) { return FCalcReturn(); }



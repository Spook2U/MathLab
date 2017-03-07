#include "MathLab.h"
#include "CalcRelation.h"

#include "Math/LinearSystem.h"
#include "CalcDistance.h"



FCalcReturn CalcRelation::CalculateWith(FMathPoint point1, FMathPoint point2)   
{ 
   FCalcReturn result;
   if(point1.coordinate == point2.coordinate) { result.relation = Relation::identical; }
   else                                       { result.relation = Relation::different; }
   return result;
}
FCalcReturn CalcRelation::CalculateWith(FMathPoint point, FMathLine line)      
{ 
   FCalcReturn result;
   if(m.IsPointInLine(line, point)) { result.relation = Relation::inside;  }
   else                             { result.relation = Relation::outside; }
   return result;
}
FCalcReturn CalcRelation::CalculateWith(FMathPoint point, FMathPlane plane)    
{ 
   FCalcReturn result;
   if(m.IsPointInPlane(plane, point)) { result.relation = Relation::inside;  }
   else                               { result.relation = Relation::outside; }
   return result;
}
FCalcReturn CalcRelation::CalculateWith(FMathPoint point, FMathSphere sphere)  
{ 
   FCalcReturn result;
   if(CalcDistance().CalculateWith(point, FMathPoint(sphere.center)).distance <= sphere.radius) { result.relation = Relation::inside;  }
   else                                                                                         { result.relation = Relation::outside; }
   return result;
}

// -------------------------------------------------------------------------------------------------

FCalcReturn CalcRelation::CalculateWith(FMathLine line, FMathPoint point)    
{
   return CalculateWith(point, line); 
}
FCalcReturn CalcRelation::CalculateWith(FMathLine line1, FMathLine line2)     
{ 
   FCalcReturn result;

   if(UKismetMathLibrary::Normal(line1.direction) == UKismetMathLibrary::Normal(line2.direction))
   {
      if(m.IsPointInLine(line1, line2.position)) { result.relation = Relation::identical; }
      else                                       { result.relation = Relation::parallel;  }
   }
   else
   {
      FLinearSystem linearSystem = FLinearSystem(FNMatrix({FNVector({line1.direction.X, (-1)*line2.direction.X, line2.position.X-line1.position.X}), 
                                                           FNVector({line1.direction.Y, (-1)*line2.direction.Y, line2.position.Y-line1.position.Y}),
                                                           FNVector({line1.direction.Z, (-1)*line2.direction.Z, line2.position.Z-line1.position.Z})
                                                          }));

      FNVector scalars;
      switch(linearSystem.GetSolution().type)
      {
         case LSSolutionType::one:       
            scalars = linearSystem.GetSolution().solution;
            if(m.GetPointOnLine(line1, scalars.Get(0)) == m.GetPointOnLine(line2, scalars.Get(1)))
            {
               result.relation = Relation::intersection;
               result.intersections = FIntersection(FMathPoint(m.GetPointOnLine(line1, scalars.Get(0))));
            }
            else
            {
               result.relation = Relation::skew;
            }
            break;
         case LSSolutionType::endless:   MLD_WAR("RelativePosition between 2 lines: result 'endless' Relation::identical? What to do here?"); break;
         case LSSolutionType::no:        result.relation = Relation::skew; break;
      }
   }
   return result;
}
FCalcReturn CalcRelation::CalculateWith(FMathLine line, FMathPlane plane)    
{ 
   FCalcReturn result;

   FLinearSystem linearSystem = FLinearSystem(FNMatrix({FNVector({plane.direction1.X, plane.direction2.X, (-1)*line.direction.X, line.position.X-plane.position.X}),
                                                        FNVector({plane.direction1.Y, plane.direction2.Y, (-1)*line.direction.Y, line.position.Y-plane.position.Y}),
                                                        FNVector({plane.direction1.Z, plane.direction2.Z, (-1)*line.direction.Z, line.position.Z-plane.position.Z})
   }));
   
   switch(linearSystem.GetSolution().type)
   {
      case LSSolutionType::one:     result.relation = Relation::intersection;
                                    result.intersections = FIntersection(m.GetPointOnLine(line, linearSystem.GetSolution().solution.Get(2))); 
      break; 
      case LSSolutionType::endless: result.relation = Relation::inside;   break;
      case LSSolutionType::no:      result.relation = Relation::parallel; break;
   }
   return result;
}
FCalcReturn CalcRelation::CalculateWith(FMathLine line, FMathSphere sphere)  
{ 
   FCalcReturn result;

   FVector  e = sphere.center - line.position;
   float    a = UKismetMathLibrary::Dot_VectorVector(e, m.MakeUnitVector(line.direction));
   float    f;
   float fSqu = sphere.radius*sphere.radius - e.Size()*e.Size() + a*a;

   if(FLOAT_EQ_ZERO(fSqu) < 0) 
   { 
      result.relation = Relation::different; 
   }
   else        
   {
      f  = FMath::Sqrt(fSqu);
      if(f == 0)  
      { 
         result.relation = Relation::intersection; 
         result.intersections = FIntersection(m.GetPointOnLine(line, a/line.direction.Size()));
      }
      else
      { 
         result.relation = Relation::intersection; 
         result.intersections = FIntersection(FPuncture(FMathPoint(m.GetPointOnLine(line, (a-f)/line.direction.Size())), FMathPoint(m.GetPointOnLine(line, (a+f)/line.direction.Size()))));
      }   
   }
   return result;
}

// -------------------------------------------------------------------------------------------------

FCalcReturn CalcRelation::CalculateWith(FMathPlane plane, FMathPoint point)    
{ 
   return CalculateWith(point, plane); 
}
FCalcReturn CalcRelation::CalculateWith(FMathPlane plane, FMathLine line)     
{ 
   return CalculateWith(line, plane); 
}
FCalcReturn CalcRelation::CalculateWith(FMathPlane plane1, FMathPlane plane2)   
{ 
   FCalcReturn result;
   
   FLinearSystem linearSystem = FLinearSystem(FNMatrix({FNVector({plane1.normal.X, plane1.normal.Y, plane1.normal.Z, plane1.d}),
                                                        FNVector({plane2.normal.X, plane2.normal.Y, plane2.normal.Z, plane2.d})
                                                       }));

   switch(linearSystem.GetSolution().type)
   {
      case LSSolutionType::endless:   
         if(m.MakeUnitVector(plane1.normal) == m.MakeUnitVector(plane2.normal))
         {
            result.relation = Relation::identical; 
         }
         else
         {
            result.relation = Relation::intersection; 
            FNMatrix resultMatrix = linearSystem.coefficientMatrix;
            FVector pos;
            FVector dir;
            
            // E: ax + by + cz = d
            // linear System Result: ky + lz = m
            // z = t
            // y = m/k - l/k * t
            // x = (-b*m + k*d) / k*a  + ((b*l - k*c) / k*a) *t 

            float a = plane1.normal.X;
            float b = plane1.normal.Y;
            float c = plane1.normal.Z;
            float d = plane1.d;
            float k = resultMatrix.GetElement(1, 1);
            float l = resultMatrix.GetElement(2, 1);
            float m = resultMatrix.GetElement(3, 1);
            
            pos = FVector(((-1)*b*m + k*d) / (k*a), m/k, 0);
            dir = FVector((b*l - k*c) / (k*a), (-1)*l/k, 1);
            
            result.intersections = FIntersection(FMathLine(pos, dir)); 
         }
      break;
      case LSSolutionType::no: result.relation = Relation::parallel; break;
   }
   return result;
}
FCalcReturn CalcRelation::CalculateWith(FMathPlane plane, FMathSphere sphere)  
{ 
   FCalcReturn result;
   float distance = CalcDistance().CalculateWith(plane, sphere).distance;

   if(distance > 0)
   {
      result.relation = Relation::different;
   }
   else
   {
      FMathLine perpLine = FMathLine(sphere.center, plane.normal);
      FMathPoint perpLineIntersection = CalculateWith(perpLine, plane).intersections.point;

      if(distance == 0)
      {
         result.relation = Relation::intersection; 
         result.intersections = FIntersection(perpLineIntersection.coordinate);
      }
      else
      {
         float dist = CalcDistance().CalculateWith(perpLineIntersection, sphere.center).distance;
         float circleRadius = m.SetOfPythagorasGetA(dist, sphere.radius);

         result.relation = Relation::intersection;
         result.intersections = FIntersection(FMathCircle(perpLineIntersection.coordinate, plane.normal, circleRadius));
      }
   }
   return result;
}

// -------------------------------------------------------------------------------------------------

FCalcReturn CalcRelation::CalculateWith(FMathSphere sphere, FMathPoint point)    
{ 
   return CalculateWith(point, sphere); 
}
FCalcReturn CalcRelation::CalculateWith(FMathSphere sphere, FMathLine line)      
{ 
   return CalculateWith(line, sphere); 
} 
FCalcReturn CalcRelation::CalculateWith(FMathSphere sphere, FMathPlane plane)    
{ 
   return CalculateWith(plane, sphere); 
}
FCalcReturn CalcRelation::CalculateWith(FMathSphere sphere1, FMathSphere sphere2) 
{
   MLD_WAR("Not defined yet");
   return FCalcReturn(Relation::undefined);
}

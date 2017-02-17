#include "MathLab.h"
#include "CalcRelation.h"

#include "Math/LinearSystem.h"
#include "CalcDistance.h"



FCalcReturn CalcRelation::CalculateWith(FMathPoint point1, FMathPoint point2)   
{ 
   FCalcReturn result;
   if(point1.Coordinate == point2.Coordinate) { result.relation = Relation::identical; }
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
   if(CalcDistance().CalculateWith(point, FMathPoint(sphere.Coordinate)).distance <= sphere.Radius) { result.relation = Relation::inside;  }
   else                                                                                             { result.relation = Relation::outside; }
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

   if(UKismetMathLibrary::Normal(line1.Direction) == UKismetMathLibrary::Normal(line2.Direction))
   {
      if(m.IsPointInLine(line1, line2.Position)) { result.relation = Relation::identical; }
      else                                       { result.relation = Relation::parallel;  }
   }
   else
   {
      FLinearSystem linearSystem = FLinearSystem(FNMatrix({FNVector({line1.Direction.X, (-1)*line2.Direction.X, line2.Position.X-line1.Position.X}), 
                                                           FNVector({line1.Direction.Y, (-1)*line2.Direction.Y, line2.Position.Y-line1.Position.Y}),
                                                           FNVector({line1.Direction.Z, (-1)*line2.Direction.Z, line2.Position.Z-line1.Position.Z})
                                                          }));

      FNVector scalars;
      switch(linearSystem.GetSolution().type)
      {
         case LSSolutionType::one:       
            scalars = linearSystem.GetSolution().solution;
            if(m.GetPointOnLine(line1, scalars.Get(0)) == m.GetPointOnLine(line2, scalars.Get(1)))
            {
               result.relation = Relation::intersection;
               result.intersections.point = FMathPoint(m.GetPointOnLine(line1, scalars.Get(0)));
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

   FLinearSystem linearSystem = FLinearSystem(FNMatrix({FNVector({plane.Direction1.X, plane.Direction2.X, (-1)*line.Direction.X, line.Position.X-plane.Position.X}),
                                                       FNVector({plane.Direction1.Y, plane.Direction2.Y, (-1)*line.Direction.Y, line.Position.Y-plane.Position.Y}),
                                                       FNVector({plane.Direction1.Z, plane.Direction2.Z, (-1)*line.Direction.Z, line.Position.Z-plane.Position.Z})
   }));
   
   switch(linearSystem.GetSolution().type)
   {
      case LSSolutionType::one:     result.relation = Relation::intersection;
                                    result.intersections.point = m.GetPointOnLine(line, linearSystem.GetSolution().solution.Get(2)); 
      break; 
      case LSSolutionType::endless: result.relation = Relation::inside;   break;
      case LSSolutionType::no:      result.relation = Relation::parallel; break;
   }
   return result;
}
FCalcReturn CalcRelation::CalculateWith(FMathLine line, FMathSphere sphere)  
{ 
   FCalcReturn result;

   FVector  e = sphere.Coordinate - line.Position;
   float    a = UKismetMathLibrary::Dot_VectorVector(e, m.MakeUnitVector(line.Direction));
   float    f;
   float fSqu = sphere.Radius*sphere.Radius - e.Size()*e.Size() + a*a;

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
         result.intersections.point = m.GetPointOnLine(line, a/line.Direction.Size());
      }
      else
      { 
         result.relation = Relation::intersection; 
         result.intersections.puncture.entry = m.GetPointOnLine(line, (a-f)/line.Direction.Size());
         result.intersections.puncture.exit  = m.GetPointOnLine(line, (a+f)/line.Direction.Size());
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
   FLinearSystem linearSystem = FLinearSystem(FNMatrix({FNVector({plane1.Direction1.X, plane1.Direction2.X, (-1)*plane2.Direction1.X, (-1)*plane2.Direction2.X, plane2.Position.X-plane1.Position.X}),
                                                        FNVector({plane1.Direction1.Y, plane1.Direction2.Y, (-1)*plane2.Direction1.Y, (-1)*plane2.Direction2.Y, plane2.Position.Y-plane1.Position.Y}),
                                                        FNVector({plane1.Direction1.Z, plane1.Direction2.Z, (-1)*plane2.Direction1.Z, (-1)*plane2.Direction2.Z, plane2.Position.Z-plane1.Position.Z})
                                                       }));
   switch(linearSystem.GetSolution().type)
   {
      case LSSolutionType::endless:   
         if(m.MakeUnitVector(plane1.Normal) == m.MakeUnitVector(plane2.Normal))
         {
            result.relation = Relation::identical; 
         }
         else
         {
            result.relation = Relation::intersection; 
            //result.intersections.line = m.GetIntersectionLine(plane2, (-1)*linearSystem.GetSolution().solution.Get(0)); 
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
      FMathLine perpLine = FMathLine(sphere.Coordinate, plane.Normal);
      FMathPoint perpLineIntersection = CalculateWith(perpLine, plane).intersections.point;

      if(distance == 0)
      {
         result.relation = Relation::intersection; 
         result.intersections.point = perpLineIntersection.Coordinate;
      }
      else
      {
         float dist = CalcDistance().CalculateWith(perpLineIntersection, sphere.Coordinate).distance;
         float circleRadius = m.SetOfPythagorasGetA(dist, sphere.Radius);

         result.relation = Relation::intersection;
         result.intersections.circle = FMathCircle(perpLineIntersection.Coordinate, plane.Normal, circleRadius);
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

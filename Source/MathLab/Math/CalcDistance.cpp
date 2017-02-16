#include "MathLab.h"
#include "CalcDistance.h"

#include "CalcRelation.h"



FCalcReturn CalcDistance::CalculateWith(FMathPoint point1, FMathPoint point2)   
{ 
   return FCalcReturn(m.VectorDistance(point2.Coordinate, point1.Coordinate)); 
}
FCalcReturn CalcDistance::CalculateWith(FMathPoint point, FMathLine line)      
{
   /* Calculated with Surface of Vectorproduct and Surface of a triangle. */ 
   FVector v = point.Coordinate - line.Position;
   return FCalcReturn(UKismetMathLibrary::VSize(FVector::CrossProduct(v, line.Direction)) / UKismetMathLibrary::VSize(line.Direction)); 
}
FCalcReturn CalcDistance::CalculateWith(FMathPoint point, FMathPlane plane)    
{ 
   return FCalcReturn(m.HesseNormalFormPlugIn(plane, point)); 
}
FCalcReturn CalcDistance::CalculateWith(FMathPoint point, FMathSphere sphere)  
{ 
   return FCalcReturn(CalculateWith(FMathPoint(sphere.Coordinate), point).distance - sphere.Radius);
}

// -------------------------------------------------------------------------------------------------

FCalcReturn CalcDistance::CalculateWith(FMathLine line, FMathPoint point)    
{ 
   return CalculateWith(point, line); 
}
FCalcReturn CalcDistance::CalculateWith(FMathLine line1, FMathLine line2)     
{ 
   FCalcReturn distance;
   switch(CalcRelation().CalculateWith(line1, line2).relation)
   {
      case Relation::identical:    
      case Relation::intersection: distance = FCalcReturn(0.f); break;
      case Relation::parallel:     distance = CalculateWith(FMathPoint(line1.Position), line2); break;
      case Relation::skew:         distance = CalculateWith(FMathPoint(line2.Position), FMathPlane(FMathPoint(line1.Position), m.MakeNomal(line1.Direction, line2.Direction))); break;
      default:                     MLD_WAR("Invalid Relative Position for 2 lines"); break;
   }

   return distance; 
}
FCalcReturn CalcDistance::CalculateWith(FMathLine line, FMathPlane plane)    
{ 
   FCalcReturn distance;
   switch(CalcRelation().CalculateWith(line, plane).relation)
   {
      case Relation::inside:       
      case Relation::intersection: distance = 0.f; break;
      case Relation::parallel:     distance = CalculateWith(FMathPoint(line.Position), plane); break;
      default:                     MLD_WAR("Wrong Relative Position for line & planes"); break;
   }

   return distance;
}
FCalcReturn CalcDistance::CalculateWith(FMathLine line, FMathSphere sphere)  
{ 
   return FCalcReturn(CalculateWith(FMathPoint(sphere.Coordinate), line).distance - sphere.Radius); 
}

// -------------------------------------------------------------------------------------------------

FCalcReturn CalcDistance::CalculateWith(FMathPlane plane, FMathPoint point)    
{ 
   return CalculateWith(point, plane); 
}
FCalcReturn CalcDistance::CalculateWith(FMathPlane plane, FMathLine line)     
{ 
   return CalculateWith(line, plane); 
}
FCalcReturn CalcDistance::CalculateWith(FMathPlane plane1, FMathPlane plane2)   
{ 
   FCalcReturn distance;
   switch(CalcRelation().CalculateWith(plane1, plane2).relation)
   {
      case Relation::identical:    
      case Relation::intersection: distance = 0.f; break;
      case Relation::parallel:     distance = CalculateWith(FMathPoint(plane1.Position), plane2); break;
      default:                     MLD_WAR("Wrong Relative Position for 2 planes"); break;
   }

   return distance;
}
FCalcReturn CalcDistance::CalculateWith(FMathPlane plane, FMathSphere sphere)  
{ 
   return FCalcReturn(CalculateWith(FMathPoint(sphere.Coordinate), plane).distance - sphere.Radius); 
}

// -------------------------------------------------------------------------------------------------

FCalcReturn CalcDistance::CalculateWith(FMathSphere sphere, FMathPoint point)    
{ 
   return CalculateWith(point, sphere); 
}
FCalcReturn CalcDistance::CalculateWith(FMathSphere sphere, FMathLine line)      
{ 
   return CalculateWith(line, sphere); 
} 
FCalcReturn CalcDistance::CalculateWith(FMathSphere sphere, FMathPlane plane)    
{ 
   return CalculateWith(plane, sphere); 
}
FCalcReturn CalcDistance::CalculateWith(FMathSphere sphere1, FMathSphere sphere2) 
{ 
   return FCalcReturn(CalculateWith(FMathPoint(sphere2.Coordinate), FMathPoint(sphere1.Coordinate)).distance - sphere1.Radius - sphere2.Radius); 
}

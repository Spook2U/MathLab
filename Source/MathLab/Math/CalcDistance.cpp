#include "MathLab.h"
#include "CalcDistance.h"

#include "CalcRelation.h"



FCalcReturn CalcDistance::CalculateWith(FMathPoint point1, FMathPoint point2)   
{ 
   return FCalcReturn(m.VectorDistance(point2.coordinate, point1.coordinate)); 
}

FCalcReturn CalcDistance::CalculateWith(FMathPoint point, FMathLine line)      
{
   /* Calculated with Surface of Vectorproduct and Surface of a triangle. */ 
   FVector v = point.coordinate - line.position;
   return FCalcReturn(UKismetMathLibrary::VSize(FVector::CrossProduct(v, line.direction)) / UKismetMathLibrary::VSize(line.direction)); 
}

FCalcReturn CalcDistance::CalculateWith(FMathPoint point, FMathPlane plane)    
{ 
   return FCalcReturn(UKismetMathLibrary::Abs(m.HesseNormalFormPlugIn(plane, point))); 
}

FCalcReturn CalcDistance::CalculateWith(FMathPoint point, FMathSphere sphere)  
{ 
   return FCalcReturn(CalculateWith(FMathPoint(sphere.center), point).distance - sphere.radius);
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------

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
      case Relation::parallel:     distance = CalculateWith(FMathPoint(line1.position), line2); break;
      case Relation::skew:         distance = CalculateWith(FMathPoint(line2.position), FMathPlane(FMathPoint(line1.position), m.MakeNomal(line1.direction, line2.direction))); break;
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
      case Relation::parallel:     distance = CalculateWith(FMathPoint(line.position), plane); break;
      default:                     MLD_WAR("Wrong Relative Position for line & plane"); break;
   }

   return distance;
}

FCalcReturn CalcDistance::CalculateWith(FMathLine line, FMathSphere sphere)  
{ 
   return FCalcReturn(CalculateWith(FMathPoint(sphere.center), line).distance - sphere.radius); 
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------

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
      case Relation::identical:    MLD_LOG("identical"); break;
      case Relation::intersection: MLD_LOG("intersection"); break;
      case Relation::parallel:     MLD_LOG("Parallel"); break;
      default:                     MLD_WAR("Wrong Relative Position for 2 planes"); break;
   }

   switch(CalcRelation().CalculateWith(plane1, plane2).relation)
   {
      case Relation::identical:    
      case Relation::intersection: distance = 0.f; break;
      case Relation::parallel:     distance = CalculateWith(FMathPoint(plane1.position), plane2); break;
      default:                     MLD_WAR("Wrong Relative Position for 2 planes"); break;
   }

   return distance;
}

FCalcReturn CalcDistance::CalculateWith(FMathPlane plane, FMathSphere sphere)  
{ 
   return FCalcReturn(CalculateWith(FMathPoint(sphere.center), plane).distance - sphere.radius); 
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------

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
   return FCalcReturn(CalculateWith(FMathPoint(sphere2.center), FMathPoint(sphere1.center)).distance - sphere1.radius - sphere2.radius); 
}

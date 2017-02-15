
#include "MathLab.h"
#include "GeometryCalc.h"

#include "MathLabEnums.h"
#include "Math/LinearSystem.h"

#include "CoordinateSystem/LineBase.h"
#include "CoordinateSystem/PlaneBase.h"
#include "CoordinateSystem/PointBase.h"
#include "CoordinateSystem/SphereBase.h"
#include "CoordinateSystem/VectorStruct.h"



#define SHIFT 16
#define SHIFTPOW 65536



FRelativePosition::FRelativePosition(){}
FRelativePosition::FRelativePosition(Relation inRelation, TArray<FVector> inIntersections)
{
   relation = inRelation;
   intersections = inIntersections;
}



GeometryCalc::GeometryCalc() {}
GeometryCalc::~GeometryCalc() {}



long GeometryCalc::GeometryTypeToLong(GeometryType type)
{
   long result = 0;
   switch(type)
   {
      case GeometryType::point:  result |= 1; break;
      case GeometryType::line:   result |= 2; break;
      case GeometryType::plane:  result |= 4; break;
      case GeometryType::sphere: result |= 8; break;
      default: break;
   }

   return result;
}



float GeometryCalc::GetDistance(AGeometryBase *from, AGeometryBase *to)
{
   if(!(MLD_PTR_CHECK(from) && MLD_PTR_CHECK(to))) return 0.f;

   float result = 0.f;

   long high  = GeometryTypeToLong(from->type);
   long low = GeometryTypeToLong(to->type);
   long mode = high << SHIFT | low;

   switch(mode)
   {
      case SHIFTPOW*1+1: result = DistanceCalc(((APointBase *)  from)->point,  ((APointBase *)  to)->point);  break;
      case SHIFTPOW*1+2: result = DistanceCalc(((APointBase *)  from)->point,  ((ALineBase *)   to)->line);   break;
      case SHIFTPOW*1+4: result = DistanceCalc(((APointBase *)  from)->point,  ((APlaneBase *)  to)->plane);  break;
      case SHIFTPOW*1+8: result = DistanceCalc(((APointBase *)  from)->point,  ((ASphereBase *) to)->sphere); break;
      case SHIFTPOW*2+1: result = DistanceCalc(((ALineBase *)   from)->line,   ((APointBase *)  to)->point);  break;
      case SHIFTPOW*2+2: result = DistanceCalc(((ALineBase *)   from)->line,   ((ALineBase *)   to)->line);   break;
      case SHIFTPOW*2+4: result = DistanceCalc(((ALineBase *)   from)->line,   ((APlaneBase *)  to)->plane);  break;
      case SHIFTPOW*2+8: result = DistanceCalc(((ALineBase *)   from)->line,   ((ASphereBase *) to)->sphere); break;
      case SHIFTPOW*4+1: result = DistanceCalc(((APlaneBase *)  from)->plane,  ((APointBase *)  to)->point);  break;
      case SHIFTPOW*4+2: result = DistanceCalc(((APlaneBase *)  from)->plane,  ((ALineBase *)   to)->line);   break;
      case SHIFTPOW*4+4: result = DistanceCalc(((APlaneBase *)  from)->plane,  ((APlaneBase *)  to)->plane);  break;
      case SHIFTPOW*4+8: result = DistanceCalc(((APlaneBase *)  from)->plane,  ((ASphereBase *) to)->sphere); break;
      case SHIFTPOW*8+1: result = DistanceCalc(((ASphereBase *) from)->sphere, ((APointBase *)  to)->point);  break;
      case SHIFTPOW*8+2: result = DistanceCalc(((ASphereBase *) from)->sphere, ((ALineBase *)   to)->line);   break;
      case SHIFTPOW*8+4: result = DistanceCalc(((ASphereBase *) from)->sphere, ((APlaneBase *)  to)->plane);  break;
      case SHIFTPOW*8+8: result = DistanceCalc(((ASphereBase *) from)->sphere, ((ASphereBase *) to)->sphere); break;
      default: break;
   }

   return result;
}


FRelativePosition GeometryCalc::GetRelativePosition(AGeometryBase *from, AGeometryBase *to)
{
   if(!(MLD_PTR_CHECK(from) && MLD_PTR_CHECK(to))) return FRelativePosition();
   FRelativePosition result;

   long high  = GeometryTypeToLong(from->type);
   long low = GeometryTypeToLong(to->type);
   long mode = high << SHIFT | low;

   switch(mode)
   {
      case SHIFTPOW*1+1: result = RelativePositionCalc(((APointBase *)  from)->point,  ((APointBase *)  to)->point);  break;
      case SHIFTPOW*1+2: result = RelativePositionCalc(((APointBase *)  from)->point,  ((ALineBase *)   to)->line);   break;
      case SHIFTPOW*1+4: result = RelativePositionCalc(((APointBase *)  from)->point,  ((APlaneBase *)  to)->plane);  break;
      case SHIFTPOW*1+8: result = RelativePositionCalc(((APointBase *)  from)->point,  ((ASphereBase *) to)->sphere); break;
      case SHIFTPOW*2+1: result = RelativePositionCalc(((ALineBase *)   from)->line,   ((APointBase *)  to)->point);  break;
      case SHIFTPOW*2+2: result = RelativePositionCalc(((ALineBase *)   from)->line,   ((ALineBase *)   to)->line);   break;
      case SHIFTPOW*2+4: result = RelativePositionCalc(((ALineBase *)   from)->line,   ((APlaneBase *)  to)->plane);  break;
      case SHIFTPOW*2+8: result = RelativePositionCalc(((ALineBase *)   from)->line,   ((ASphereBase *) to)->sphere); break;
      case SHIFTPOW*4+1: result = RelativePositionCalc(((APlaneBase *)  from)->plane,  ((APointBase *)  to)->point);  break;
      case SHIFTPOW*4+2: result = RelativePositionCalc(((APlaneBase *)  from)->plane,  ((ALineBase *)   to)->line);   break;
      case SHIFTPOW*4+4: result = RelativePositionCalc(((APlaneBase *)  from)->plane,  ((APlaneBase *)  to)->plane);  break;
      case SHIFTPOW*4+8: result = RelativePositionCalc(((APlaneBase *)  from)->plane,  ((ASphereBase *) to)->sphere); break;
      case SHIFTPOW*8+1: result = RelativePositionCalc(((ASphereBase *) from)->sphere, ((APointBase *)  to)->point);  break;
      case SHIFTPOW*8+2: result = RelativePositionCalc(((ASphereBase *) from)->sphere, ((ALineBase *)   to)->line);   break;
      case SHIFTPOW*8+4: result = RelativePositionCalc(((ASphereBase *) from)->sphere, ((APlaneBase *)  to)->plane);  break;
      case SHIFTPOW*8+8: result = RelativePositionCalc(((ASphereBase *) from)->sphere, ((ASphereBase *) to)->sphere); break;
      default: break;
   }

   return result;
}

bool GeometryCalc::IsPointInLine(FMathLine line, FMathPoint point)
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

bool GeometryCalc::IsPointInPlane(FMathPlane plane, FMathPoint point)
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

FVector GeometryCalc::GetPointOnLine(FMathLine line, float scalar)
{
   return line.Position + scalar * line.Direction;
}

FVector GeometryCalc::GetPointOnPlane(FMathPlane plane, float scalar1, float scalar2)
{
   return plane.Position + scalar1 * plane.Direction1 + scalar2 * plane.Direction2;
}

TArray<FVector> GeometryCalc::GetIntersectionLine(FMathPlane plane, float u)
{
   TArray<FVector> line;
   if(u != 0) { line = {plane.Position, plane.Direction1 + plane.Direction2 / u}; }
   else       { MLD_ERR("u = 0; Division through 0 not allowed."); }       
   return line;
}

float GeometryCalc::SetOfPythagorasGetA(float b, float c)
{
   return FMath::Sqrt(c*c - b*b);
}

float GeometryCalc::SetOfPythagorasGetB(float a, float c)
{
   return FMath::Sqrt(c*c - a*a);
}

float GeometryCalc::SetOfPythagorasGetC(float a, float b)
{
   return FMath::Sqrt(a*a + b*b);
}

float GeometryCalc::HesseNormalFormPlugIn(FMathPlane plane, FMathPoint point)
{
   MLD_LOG("HesseNormalFormPlugIn: Point: %s   +   Plane: %s, %s, %s   = %f", *point.Coordinate.ToString(), *plane.Position.ToString(), *plane.Direction1.ToString(), *plane.Direction2.ToString(), UKismetMathLibrary::Dot_VectorVector(plane.Normal, point.Coordinate) - UKismetMathLibrary::Dot_VectorVector(plane.Normal, plane.Position));
   return UKismetMathLibrary::Dot_VectorVector(plane.Normal, point.Coordinate) - UKismetMathLibrary::Dot_VectorVector(plane.Normal, plane.Position);
}

float GeometryCalc::VectorDistance(FVector a, FVector b)
{
   return UKismetMathLibrary::VSize(b-a);
}

FVector GeometryCalc::MakeNomal(FVector a, FVector b, bool unitVector)
{
   FVector normal = UKismetMathLibrary::Cross_VectorVector(a, b);
   if(unitVector)
   {
      MakeUnitVector(normal);
   }
   return normal;
}

FVector GeometryCalc::MakeUnitVector(FVector v)
{
   return UKismetMathLibrary::Normal(v);
}



float GeometryCalc::DistanceCalc(FMathPoint point1, FMathPoint point2)
{
   return VectorDistance(point2.Coordinate, point1.Coordinate);
}
float GeometryCalc::DistanceCalc(FMathPoint point, FMathLine line)
{
   /* Calculated with Surface of Vectorproduct and Surface of a triangle. */ 
   FVector v = point.Coordinate - line.Position;
   return UKismetMathLibrary::VSize(FVector::CrossProduct(v, line.Direction)) / UKismetMathLibrary::VSize(line.Direction);
}
float GeometryCalc::DistanceCalc(FMathPoint point, FMathPlane plane)
{
   return HesseNormalFormPlugIn(plane, point);
}
float GeometryCalc::DistanceCalc(FMathPoint point, FMathSphere sphere)
{
   return DistanceCalc(FMathPoint(sphere.Coordinate), point) - sphere.Radius;
}
float GeometryCalc::DistanceCalc(FMathLine line, FMathPoint point)
{
   return DistanceCalc(point, line);
}
float GeometryCalc::DistanceCalc(FMathLine line1, FMathLine line2)
{
   float distance = 0.f;
   switch(RelativePositionCalc(line1, line2).relation)
   {
      case Relation::identical:    distance = 0.f; break;
      case Relation::intersection: distance = 0.f; break;
      case Relation::parallel:     distance = DistanceCalc(FMathPoint(line1.Position), line2); break;
      case Relation::skew:         distance = DistanceCalc(FMathPoint(line2.Position), FMathPlane(FMathPoint(line1.Position), MakeNomal(line1.Direction, line2.Direction))); break;
      default: MLD_WAR("Wrong Relative Position for 2 lines"); break;
   }

   return distance;
}
float GeometryCalc::DistanceCalc(FMathLine line, FMathPlane plane)
{
   float distance = 0.f;
   switch(RelativePositionCalc(line, plane).relation)
   {
      case Relation::inside:       distance = 0.f; break;
      case Relation::intersection: distance = 0.f; break;
      case Relation::parallel:     distance = DistanceCalc(FMathPoint(line.Position), plane); break;
      default: MLD_WAR("Wrong Relative Position for line & planes"); break;
   }

   return distance;
}
float GeometryCalc::DistanceCalc(FMathLine line, FMathSphere sphere)
{
   return DistanceCalc(FMathPoint(sphere.Coordinate), line) - sphere.Radius;
}
float GeometryCalc::DistanceCalc(FMathPlane plane, FMathPoint point)
{
   return DistanceCalc(point, plane);
}
float GeometryCalc::DistanceCalc(FMathPlane plane, FMathLine line)
{
   return DistanceCalc(line, plane);
}
float GeometryCalc::DistanceCalc(FMathPlane plane1, FMathPlane plane2)
{
   float distance = 0.f;
   switch(RelativePositionCalc(plane1, plane2).relation)
   {
      case Relation::identical:    distance = 0.f; break;
      case Relation::intersection: distance = 0.f; break;
      case Relation::parallel:     distance = DistanceCalc(FMathPoint(plane1.Position), plane2); break;
      default: MLD_WAR("Wrong Relative Position for 2 planes"); break;
   }

   return distance;
}
float GeometryCalc::DistanceCalc(FMathPlane plane, FMathSphere sphere)
{
   return DistanceCalc(FMathPoint(sphere.Coordinate), plane) - sphere.Radius;
}
float GeometryCalc::DistanceCalc(FMathSphere sphere, FMathPoint point)
{
   return DistanceCalc(point, sphere);
}
float GeometryCalc::DistanceCalc(FMathSphere sphere, FMathLine line)
{
   return DistanceCalc(line, sphere);
}
float GeometryCalc::DistanceCalc(FMathSphere sphere, FMathPlane plane)
{
   return DistanceCalc(plane, sphere);
}
float GeometryCalc::DistanceCalc(FMathSphere sphere1, FMathSphere sphere2)
{
   return DistanceCalc(FMathPoint(sphere2.Coordinate), FMathPoint(sphere1.Coordinate)) - sphere1.Radius - sphere2.Radius;
}



FRelativePosition GeometryCalc::RelativePositionCalc(FMathPoint point1, FMathPoint point2)
{
   FRelativePosition result;
   if(point1.Coordinate == point2.Coordinate) { result.relation = Relation::identical; }
   else                                       { result.relation = Relation::different; }
   return result;
}
FRelativePosition GeometryCalc::RelativePositionCalc(FMathPoint point, FMathLine line)
{
   FRelativePosition result;
   if(IsPointInLine(line, point)) { result.relation = Relation::inside;  }
   else                           { result.relation = Relation::outside; }
   return result;
}
FRelativePosition GeometryCalc::RelativePositionCalc(FMathPoint point, FMathPlane plane)
{
   FRelativePosition result;
   if(IsPointInPlane(plane, point)) { result.relation = Relation::inside;  }
   else                             { result.relation = Relation::outside; }
   return result;
}
FRelativePosition GeometryCalc::RelativePositionCalc(FMathPoint point, FMathSphere sphere)
{
   FRelativePosition result;
   if(DistanceCalc(point, FMathPoint(sphere.Coordinate)) <= sphere.Radius) { result.relation = Relation::inside;  }
   else                                                                    { result.relation = Relation::outside; }
   return result;
}
FRelativePosition GeometryCalc::RelativePositionCalc(FMathLine  line, FMathPoint point)
{
   return RelativePositionCalc(point, line);
}
FRelativePosition GeometryCalc::RelativePositionCalc(FMathLine  line1, FMathLine line2)
{
   FRelativePosition result;
   
   if(UKismetMathLibrary::Normal(line1.Direction) == UKismetMathLibrary::Normal(line2.Direction))
   {
      if(IsPointInLine(line1, line2.Position)) { result = FRelativePosition(Relation::identical); }
      else                                     { result = FRelativePosition(Relation::parallel);  }
   }
   else
   {
      FLinearSystem linearSystem = FLinearSystem(FNMatrix({FNVector({line1.Direction.X, (-1)*line2.Direction.X, line2.Position.X-line1.Position.X}), 
                                                           FNVector({line1.Direction.Y, (-1)*line2.Direction.Y, line2.Position.Y-line1.Position.Y})
                                                          }));

      FNVector scalars;
      switch(linearSystem.GetSolution().type)
      {
         case LSSolutionType::one:       
            scalars = linearSystem.GetSolution().solution;
            if(GetPointOnLine(line1, scalars.Get(0)) == GetPointOnLine(line2, scalars.Get(1)))
            {
               result = FRelativePosition(Relation::intersection, {GetPointOnLine(line1, scalars.Get(0))});
            }
            else
            {
               result = FRelativePosition(Relation::skew);
            }
         break;
         case LSSolutionType::endless:   MLD_WAR("RelativePosition between 2 lines: result 'endless' Relation::identical? What to do here?"); break;
         case LSSolutionType::no:        result = FRelativePosition(Relation::skew); break;
      }
   }
   return result;
}
FRelativePosition GeometryCalc::RelativePositionCalc(FMathLine  line, FMathPlane plane)
{
   FRelativePosition result;
   
   FLinearSystem linearSystem = FLinearSystem(FNMatrix({FNVector({plane.Direction1.X, plane.Direction2.X, (-1)*line.Direction.X, line.Position.X-plane.Position.X}),
                                                        FNVector({plane.Direction1.Y, plane.Direction2.Y, (-1)*line.Direction.Y, line.Position.Y-plane.Position.Y}),
                                                        FNVector({plane.Direction1.Z, plane.Direction2.Z, (-1)*line.Direction.Z, line.Position.Z-plane.Position.Z})
                                                       }));
   switch(linearSystem.GetSolution().type)
   {
      case LSSolutionType::one:     result = FRelativePosition(Relation::intersection, {GetPointOnLine(line, linearSystem.GetSolution().solution.Get(2))}); break;
      case LSSolutionType::endless: result = FRelativePosition(Relation::inside);   break;
      case LSSolutionType::no:      result = FRelativePosition(Relation::parallel); break;
   }
   return result;
}
FRelativePosition GeometryCalc::RelativePositionCalc(FMathLine  line, FMathSphere sphere)
{
   FRelativePosition result;

   FVector e  = sphere.Coordinate - line.Position;
   float   a  = UKismetMathLibrary::Dot_VectorVector(e, MakeUnitVector(line.Direction));
   float   f;
   float fSqu = sphere.Radius*sphere.Radius - e.Size()*e.Size() + a*a;

   if(fSqu < 0) 
   { 
      result = FRelativePosition(Relation::different); 
   }
   else        
   {
      f  = FMath::Sqrt(fSqu);
      if(f == 0)   { result = FRelativePosition(Relation::intersection, {GetPointOnLine(line, a/line.Direction.Size())}); }
      else         { result = FRelativePosition(Relation::intersection, {GetPointOnLine(line, (a-f)/line.Direction.Size()), GetPointOnLine(line, (a+f)/line.Direction.Size())}); }
   }   
      
   return result;
}
FRelativePosition GeometryCalc::RelativePositionCalc(FMathPlane plane, FMathPoint point)
{
   return RelativePositionCalc(point, plane);
}
FRelativePosition GeometryCalc::RelativePositionCalc(FMathPlane plane, FMathLine line)
{
   return RelativePositionCalc(line, plane);
}
FRelativePosition GeometryCalc::RelativePositionCalc(FMathPlane plane1, FMathPlane plane2)
{
   FRelativePosition result;
   FLinearSystem linearSystem = FLinearSystem(FNMatrix({FNVector({plane1.Direction1.X, plane1.Direction2.X, (-1)*plane2.Direction1.X, (-1)*plane2.Direction2.X, plane2.Position.X-plane1.Position.X}),
                                                        FNVector({plane1.Direction1.Y, plane1.Direction2.Y, (-1)*plane2.Direction1.Y, (-1)*plane2.Direction2.Y, plane2.Position.Y-plane1.Position.Y}),
                                                        FNVector({plane1.Direction1.Z, plane1.Direction2.Z, (-1)*plane2.Direction1.Z, (-1)*plane2.Direction2.Z, plane2.Position.X-plane1.Position.Z})
                                                       }));
   switch(linearSystem.GetSolution().type)
   {
      case LSSolutionType::parameter: result = FRelativePosition(Relation::intersection, GetIntersectionLine(plane2, (-1)*linearSystem.GetSolution().solution.Get(0))); break;
      case LSSolutionType::endless:   result = FRelativePosition(Relation::identical); break;
      case LSSolutionType::no:        result = FRelativePosition(Relation::parallel); break;
   }
   return result;
}
FRelativePosition GeometryCalc::RelativePositionCalc(FMathPlane plane, FMathSphere sphere)
{
   FRelativePosition result;
   float distance = DistanceCalc(plane, sphere);
   
   if(distance > 0)
   {
      result = FRelativePosition(Relation::different);
   }
   else
   {
      FMathLine perpLine = FMathLine(sphere.Coordinate, plane.Normal);
      FMathPoint perpLineIntersection = FMathPoint(RelativePositionCalc(perpLine, plane).intersections[0]);

      if(distance == 0)
      {
         result = FRelativePosition(Relation::intersection, {perpLineIntersection.Coordinate});
      }
      else
      {
         float dist = DistanceCalc(perpLineIntersection, sphere.Coordinate);

         float circleRadius = SetOfPythagorasGetA(dist, sphere.Radius);
         
         MLD_LOG("center: %s, radius: %f, normal: %s", *perpLineIntersection.Coordinate.ToString(), circleRadius, *plane.Normal.ToString());

         result = FRelativePosition(Relation::intersection);
      }
   }

   return result;
}
FRelativePosition GeometryCalc::RelativePositionCalc(FMathSphere sphere, FMathPoint point)
{
   return RelativePositionCalc(point, sphere);
}
FRelativePosition GeometryCalc::RelativePositionCalc(FMathSphere sphere, FMathLine line)
{
   return RelativePositionCalc(line, sphere);
}
FRelativePosition GeometryCalc::RelativePositionCalc(FMathSphere sphere, FMathPlane plane)
{
   return RelativePositionCalc(plane, sphere);
}
FRelativePosition GeometryCalc::RelativePositionCalc(FMathSphere sphere1, FMathSphere sphere2)
{
   FRelativePosition result;
   MLD_WAR("Not defined yet");
   return result;
}


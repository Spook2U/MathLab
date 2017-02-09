
#include "MathLab.h"
#include "CoordinateSystem/GeometryBase.h"
#include "CoordinateSystem/LineBase.h"
#include "CoordinateSystem/PlaneBase.h"
#include "CoordinateSystem/PointBase.h"
#include "CoordinateSystem/SphereBase.h"
#include "CoordinateSystem/UnitBase.h"
#include "CoordinateSystem/VectorStruct.h"

#include "MathLabEnumLibrary.h"

#include "GeometryCalc.h"

#define SHIFT 16
#define SHIFTPOW 65536

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

FRelativePosition GeometryCalc::GetRelativePosition(AGeometryBase * from, AGeometryBase * to)
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

bool GeometryCalc::PointInLine(FMathLine line, FMathPoint point)
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

bool GeometryCalc::PointInPlane(FMathPlane plane, FMathPoint point)
{
   bool isInPlane = false;
   //FLinearSystem eqalation = FLinearSystem(FNMatrix({FNVector({plane.Direction1.X, plane.Direction1.Y}), 
   //                                                  FNVector({plane.Direction2.X, plane.Direction2.Y}), 
   //                                                  FNVector({point.Coordinate.X-plane.Position.X, point.Coordinate.Y-plane.Position.Y})
   //                                                 }));
   //eqalation.Solve();
   //LSSolutionType les = eqalation.HasSolution();
   //if(les == LSSolutionType::one)
   //{
   //   FNVector solution = eqalation.GetSolutionOne();
   //   
   //   if(solution.Size() != 2) { MLD_ERR("solution Vector has wrong size: %s", *solution.ToString()); return false; }

   //   if(point.Coordinate.Z == (plane.Position.Z + solution.Get(0) * plane.Direction1.Z + solution.Get(1) * plane.Direction2.Z))
   //   {
   //      isInPlane = true;
   //   }

   //}
   //else if(les == LSSolutionType::endless)
   //{
   //   MLD_WAR("PointInPlane() result 'endless' What to do here?");
   //}
   return isInPlane;
}

float GeometryCalc::HesseNormalFormPlugIn(FMathPlane plane, FMathPoint point)
{
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
      UKismetMathLibrary::Normal(normal);
   }
   return normal;
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
float GeometryCalc::DistanceCalc(FMathLine  line, FMathPoint point)
{
   return DistanceCalc(point, line);
}
float GeometryCalc::DistanceCalc(FMathLine  line1, FMathLine line2)
{
   FRelativePosition relpos = RelativePositionCalc(line1, line2);
   float distance = 0.f;

   switch(relpos.relation)
   {
      case Relation::identical:    distance = 0.f; break;
      case Relation::intersection: distance = 0.f; break;
      case Relation::parallel:     distance = DistanceCalc(FMathPoint(line1.Position), line2); break;
      case Relation::skew:         distance = DistanceCalc(FMathPoint(line2.Position), FMathPlane(FMathPoint(line1.Position), MakeNomal(line1.Direction, line2.Direction))); break;
      default: MLD_WAR("Wrong Relative Position for 2 lines"); break;
   }

   return distance;
}
float GeometryCalc::DistanceCalc(FMathLine  line, FMathPlane plane)
{
   FRelativePosition relpos = RelativePositionCalc(line, plane);
   float distance = 0.f;

   switch(relpos.relation)
   {
      case Relation::inside:       distance = 0.f; break;
      case Relation::intersection: distance = 0.f; break;
      case Relation::parallel:     distance = DistanceCalc(FMathPoint(line.Position), plane); break;
      default: MLD_WAR("Wrong Relative Position for 2 planes"); break;
   }

   return distance;
}
float GeometryCalc::DistanceCalc(FMathLine  line, FMathSphere sphere)
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
   FRelativePosition relpos = RelativePositionCalc(plane1, plane2);
   float distance = 0.f;

   switch(relpos.relation)
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
   if(PointInLine(line, point)) { result.relation = Relation::inside;  }
   else                         { result.relation = Relation::outside; }
   return result;
}
FRelativePosition GeometryCalc::RelativePositionCalc(FMathPoint point, FMathPlane plane)
{
   FRelativePosition result;
   if(PointInPlane(plane, point)) { result.relation = Relation::inside;  }
   else                           { result.relation = Relation::outside; }
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
      if(line1.Position == line2.Position) { result.relation = Relation::identical; }
      else                                 { result.relation = Relation::parallel;  }
   }

   //FLinearSystem eqalation = FLinearSystem(FNMatrix({FNVector({line1.Direction.X, line1.Direction.Y}), 
   //                                                          FNVector({(-1)*line2.Direction.X, (-1)*line2.Direction.Y}), 
   //                                                          FNVector({line1.Position.X-line2.Position.X, line1.Position.Y-line2.Position.Y})
   //                                                         }));
   //eqalation.Solve();
   //LSSolutionType les = eqalation.HasSolution();
   //if(les == LSSolutionType::one)
   //{
   //   FNVector solution = eqalation.GetSolutionOne();

   //   if(solution.Size() != 2) { MLD_ERR("solution Vector has wrong size: %s", *solution.ToString()); return FRelativePosition(); }

   //   if(line1.Position + solution.Get(0) * line1.Direction == line2.Position + solution.Get(1) * line2.Direction)
   //   {
   //      result.relation = Relation::intersection;
   //      result.intersections.Add(eqalation.GetSolutionOne().GetFVector());
   //   }
   //   else
   //   {
   //      result.relation = Relation::skew;
   //   }
   //}
   //else if(les == LSSolutionType::endless)
   //{
   //   MLD_WAR("PointInPlane() result 'endless' What to do here?");
   //}
   //else if(les == LSSolutionType::no)
   //{
   //   result.relation = Relation::skew;
   //}

   return result;
}
FRelativePosition GeometryCalc::RelativePositionCalc(FMathLine  line, FMathPlane plane)
{
   FRelativePosition result;

   return result;
}
FRelativePosition GeometryCalc::RelativePositionCalc(FMathLine  line, FMathSphere sphere)
{
   FRelativePosition result;

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

   return result;
}
FRelativePosition GeometryCalc::RelativePositionCalc(FMathPlane plane, FMathSphere sphere)
{
   FRelativePosition result;

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

   return result;
}


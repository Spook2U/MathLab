
#include "MathLab.h"
#include "CoordinateSystem/GeometryBase.h"
#include "CoordinateSystem/LineBase.h"
#include "CoordinateSystem/PlaneBase.h"
#include "CoordinateSystem/PointBase.h"
#include "CoordinateSystem/SphereBase.h"
#include "CoordinateSystem/UnitBase.h"
#include "CoordinateSystem/VectorStruct.h"

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
      case SHIFTPOW*1+1: result = DistanceCalc((APointBase *)  from, (APointBase *)  to); break;
      case SHIFTPOW*1+2: result = DistanceCalc((APointBase *)  from, (ALineBase *)   to); break;
      case SHIFTPOW*1+4: result = DistanceCalc((APointBase *)  from, (APlaneBase *)  to); break;
      case SHIFTPOW*1+8: result = DistanceCalc((APointBase *)  from, (ASphereBase *) to); break;
      case SHIFTPOW*2+1: result = DistanceCalc((ALineBase *)   from, (APointBase *)  to); break;
      case SHIFTPOW*2+2: result = DistanceCalc((ALineBase *)   from, (ALineBase *)   to); break;
      case SHIFTPOW*2+4: result = DistanceCalc((ALineBase *)   from, (APlaneBase *)  to); break;
      case SHIFTPOW*2+8: result = DistanceCalc((ALineBase *)   from, (ASphereBase *) to); break;
      case SHIFTPOW*4+1: result = DistanceCalc((APlaneBase *)  from, (APointBase *)  to); break;
      case SHIFTPOW*4+2: result = DistanceCalc((APlaneBase *)  from, (ALineBase *)   to); break;
      case SHIFTPOW*4+4: result = DistanceCalc((APlaneBase *)  from, (APlaneBase *)  to); break;
      case SHIFTPOW*4+8: result = DistanceCalc((APlaneBase *)  from, (ASphereBase *) to); break;
      case SHIFTPOW*8+1: result = DistanceCalc((ASphereBase *) from, (APointBase *)  to); break;
      case SHIFTPOW*8+2: result = DistanceCalc((ASphereBase *) from, (ALineBase *)   to); break;
      case SHIFTPOW*8+4: result = DistanceCalc((ASphereBase *) from, (APlaneBase *)  to); break;
      case SHIFTPOW*8+8: result = DistanceCalc((ASphereBase *) from, (ASphereBase *) to); break;
      default: break;
   }

   return result;
}

RelPosReturn GeometryCalc::GetRelativePosition(AGeometryBase * from, AGeometryBase * to)
{
   if(!(MLD_PTR_CHECK(from) && MLD_PTR_CHECK(to))) return RelPosReturn();
   RelPosReturn result;

   long high  = GeometryTypeToLong(from->type);
   long low = GeometryTypeToLong(to->type);
   long mode = high << SHIFT | low;

   switch(mode)
   {
      case SHIFTPOW*1+1: result = RelativePositionCalc((APointBase *)  from, (APointBase *)  to); break;
      case SHIFTPOW*1+2: result = RelativePositionCalc((APointBase *)  from, (ALineBase *)   to); break;
      case SHIFTPOW*1+4: result = RelativePositionCalc((APointBase *)  from, (APlaneBase *)  to); break;
      case SHIFTPOW*1+8: result = RelativePositionCalc((APointBase *)  from, (ASphereBase *) to); break;
      case SHIFTPOW*2+1: result = RelativePositionCalc((ALineBase *)   from, (APointBase *)  to); break;
      case SHIFTPOW*2+2: result = RelativePositionCalc((ALineBase *)   from, (ALineBase *)   to); break;
      case SHIFTPOW*2+4: result = RelativePositionCalc((ALineBase *)   from, (APlaneBase *)  to); break;
      case SHIFTPOW*2+8: result = RelativePositionCalc((ALineBase *)   from, (ASphereBase *) to); break;
      case SHIFTPOW*4+1: result = RelativePositionCalc((APlaneBase *)  from, (APointBase *)  to); break;
      case SHIFTPOW*4+2: result = RelativePositionCalc((APlaneBase *)  from, (ALineBase *)   to); break;
      case SHIFTPOW*4+4: result = RelativePositionCalc((APlaneBase *)  from, (APlaneBase *)  to); break;
      case SHIFTPOW*4+8: result = RelativePositionCalc((APlaneBase *)  from, (ASphereBase *) to); break;
      case SHIFTPOW*8+1: result = RelativePositionCalc((ASphereBase *) from, (APointBase *)  to); break;
      case SHIFTPOW*8+2: result = RelativePositionCalc((ASphereBase *) from, (ALineBase *)   to); break;
      case SHIFTPOW*8+4: result = RelativePositionCalc((ASphereBase *) from, (APlaneBase *)  to); break;
      case SHIFTPOW*8+8: result = RelativePositionCalc((ASphereBase *) from, (ASphereBase *) to); break;
      default: break;
   }

   return result;
}

float GeometryCalc::HesseNormalFormPlugIn(APlaneBase *plane, FVector point)
{
   if(!MLD_PTR_CHECK(plane))  return 0.f;
   return UKismetMathLibrary::Dot_VectorVector(plane->Normal, point) - UKismetMathLibrary::Dot_VectorVector(plane->Normal, plane->Position);
}

float GeometryCalc::HesseNormalFormPlugIn(APlaneBase *plane, APointBase *point)
{
   if(!MLD_PTR_CHECK(point))  return 0.f;
   return HesseNormalFormPlugIn(plane, point->point.Coordinate);
}



float GeometryCalc::VectorDistance(FVector v1, FVector v2)
{
   return UKismetMathLibrary::VSize(v2-v1);
}



float GeometryCalc::DistanceCalc(APointBase *point1, APointBase *point2)
{
   return VectorDistance(point2->point.Coordinate, point1->point.Coordinate);
}
float GeometryCalc::DistanceCalc(APointBase *point, ALineBase *line)
{
   /* surface of vectorproduct: surface = 1/2 * (Vector from Position to point) crossprudct direction
      heigh of triange: surface = 1/2 * g * h
      g = size of direction
      h = searched distance d
      -> 1/2 * size of diection * d = 1/2 * (Vector from Position to point) crossprudct direction
      -> d = (Vector from Position to point) crossprudct direction / size of diection
   */ 
   FVector v = point->point.Coordinate - line->Position;
   return UKismetMathLibrary::VSize(FVector::CrossProduct(v, line->Direction)) / UKismetMathLibrary::VSize(line->Direction);
}
float GeometryCalc::DistanceCalc(APointBase *point, APlaneBase *plane)
{
   return HesseNormalFormPlugIn(plane, point);
}
float GeometryCalc::DistanceCalc(APointBase *point, ASphereBase *sphere)
{
   return VectorDistance(sphere->point.Coordinate, point->point.Coordinate) - sphere->Radius;
}
float GeometryCalc::DistanceCalc(ALineBase *line, APointBase *point)
{
   return DistanceCalc(point, line);
}
float GeometryCalc::DistanceCalc(ALineBase *line1, ALineBase *line2)
{
   RelPosReturn relpos = GetRelativePosition(line1, line2);
   float distance = 0.f;

   switch(relpos.type)
   {
      case RelativePosition::identical:    distance = 0.f; break;
      case RelativePosition::intersection: distance = 0.f; break;
      case RelativePosition::parallel:     /* gleich wie: DistanceCalc(APointBase *point, ALineBase *line) Abstand p1 von g2 */ break;
      case RelativePosition::skew:         /* distance = HesseNormalFormPlugIn();*/ break;
                                           /* gleich wie: DistanceCalc(APointBase *point, APlaneBase *plane): d1 x d2 = n, p1 & n = Ebene E, Abstand von E und p2*/
      default: MLD_WAR("Wrong Relative Position for 2 lines"); break;
   }

   return distance;
}
float GeometryCalc::DistanceCalc(ALineBase *line, APlaneBase *plane)
{
   RelPosReturn relpos = GetRelativePosition(line, plane);
   float distance = 0.f;

   switch(relpos.type)
   {
      case RelativePosition::inside:       distance = 0.f; break;
      case RelativePosition::intersection: distance = 0.f; break;
      case RelativePosition::parallel:     /* gleich wie: DistanceCalc(APointBase *point, APlaneBase *plane) Abstand Gerade p1 von E1 */ break;
      default: MLD_WAR("Wrong Relative Position for 2 planes"); break;
   }

   return distance;
}
float GeometryCalc::DistanceCalc(ALineBase *line, ASphereBase *sphere)
{
   /* gleich wie: DistanceCalc(APointBase *point, ALineBase *line) Abstand p1 von g2 */
   // Dann - radius
   return 5.0f;
}
float GeometryCalc::DistanceCalc(APlaneBase *plane, APointBase *point)
{
   return DistanceCalc(point, plane);
}
float GeometryCalc::DistanceCalc(APlaneBase *plane, ALineBase *line)
{
   return DistanceCalc(line, plane);
}
float GeometryCalc::DistanceCalc(APlaneBase *plane1, APlaneBase *plane2)
{
   RelPosReturn relpos = GetRelativePosition(plane1, plane2);
   float distance = 0.f;

   switch(relpos.type)
   {
      case RelativePosition::identical:    distance = 0.f; break;
      case RelativePosition::intersection: distance = 0.f; break;
      case RelativePosition::parallel:     /* gleich wie: DistanceCalc(APointBase *point, APlaneBase *plane) Abstand p1 von E2 */ break;
      default: MLD_WAR("Wrong Relative Position for 2 planes"); break;
   }

   return distance;
}
float GeometryCalc::DistanceCalc(APlaneBase *plane, ASphereBase *sphere)
{
   /* gleich wie: DistanceCalc(APointBase *point, APlaneBase *plane) Abstand p1 von E2 */
   // Dann - radius
   return 7.0f;
}
float GeometryCalc::DistanceCalc(ASphereBase *sphere, APointBase *point)
{
   return DistanceCalc(point, sphere);
}
float GeometryCalc::DistanceCalc(ASphereBase *sphere, ALineBase *line)
{
   return DistanceCalc(line, sphere);
}
float GeometryCalc::DistanceCalc(ASphereBase *sphere, APlaneBase *plane)
{
   return DistanceCalc(plane, sphere);
}
float GeometryCalc::DistanceCalc(ASphereBase *sphere1, ASphereBase *sphere2)
{
   return VectorDistance(sphere2->point.Coordinate, sphere1->point.Coordinate) - sphere1->Radius - sphere2->Radius;
}



RelPosReturn GeometryCalc::RelativePositionCalc(APointBase *point1, APointBase *point2)
{
   RelPosReturn result;
   if(point1->point.Coordinate == point2->point.Coordinate) { result.type = RelativePosition::identical; }
   else                                                     { result.type = RelativePosition::different; }
   return result;
}
RelPosReturn GeometryCalc::RelativePositionCalc(APointBase *point, ALineBase *line)
{
   RelPosReturn result;

   return result;
}
RelPosReturn GeometryCalc::RelativePositionCalc(APointBase *point, APlaneBase *plane)
{
   RelPosReturn result;

   return result;
}
RelPosReturn GeometryCalc::RelativePositionCalc(APointBase *point, ASphereBase *sphere)
{
   RelPosReturn result;
   if(VectorDistance(point->point.Coordinate, sphere->point.Coordinate) <= sphere->Radius) { result.type = RelativePosition::inside;  }
   else                                                                                    { result.type = RelativePosition::outside; }
   return result;
}
RelPosReturn GeometryCalc::RelativePositionCalc(ALineBase *line, APointBase *point)
{
   return RelativePositionCalc(point, line);
}
RelPosReturn GeometryCalc::RelativePositionCalc(ALineBase *line1, ALineBase *line2)
{
   RelPosReturn result;

   return result;
}
RelPosReturn GeometryCalc::RelativePositionCalc(ALineBase *line, APlaneBase *plane)
{
   RelPosReturn result;

   return result;
}
RelPosReturn GeometryCalc::RelativePositionCalc(ALineBase *line, ASphereBase *sphere)
{
   RelPosReturn result;

   return result;
}
RelPosReturn GeometryCalc::RelativePositionCalc(APlaneBase *plane, APointBase *point)
{
   return RelativePositionCalc(point, plane);
}
RelPosReturn GeometryCalc::RelativePositionCalc(APlaneBase *plane, ALineBase *line)
{
   return RelativePositionCalc(line, plane);
}
RelPosReturn GeometryCalc::RelativePositionCalc(APlaneBase *plane1, APlaneBase *plane2)
{
   RelPosReturn result;

   return result;
}
RelPosReturn GeometryCalc::RelativePositionCalc(APlaneBase *plane, ASphereBase *sphere)
{
   RelPosReturn result;

   return result;
}
RelPosReturn GeometryCalc::RelativePositionCalc(ASphereBase *sphere, APointBase *point)
{
   return RelativePositionCalc(point, sphere);
}
RelPosReturn GeometryCalc::RelativePositionCalc(ASphereBase *sphere, ALineBase *line)
{
   return RelativePositionCalc(line, sphere);
}
RelPosReturn GeometryCalc::RelativePositionCalc(ASphereBase *sphere, APlaneBase *plane)
{
   return RelativePositionCalc(plane, sphere);
}
RelPosReturn GeometryCalc::RelativePositionCalc(ASphereBase *sphere1, ASphereBase *sphere2)
{
   RelPosReturn result;

   return result;
}


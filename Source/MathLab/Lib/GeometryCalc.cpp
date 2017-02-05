
#include "MathLab.h"
#include "CoordinateSystem/GeometryBase.h"
#include "CoordinateSystem/LineBase.h"
#include "CoordinateSystem/PlaneBase.h"
#include "CoordinateSystem/PointBase.h"
#include "CoordinateSystem/SphereBase.h"
#include "CoordinateSystem/UnitBase.h"
#include "CoordinateSystem/VectorStruct.h"

#include "GeometryCalc.h"

GeometryCalc::GeometryCalc()
{}

GeometryCalc::~GeometryCalc()
{}



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



float GeometryCalc::Distance(AGeometryBase * from, AGeometryBase * to)
{
   float result = 0.f;

   long low  = GeometryTypeToLong(from->type);
   long high = GeometryTypeToLong(to->type);
   long mode = high << 16 | low;
   
   switch(mode)
   {
      case 65536*1+1: result = DistanceCalc((APointBase *)  from, (APointBase *)  to);   break;
      case 65536*1+2: result = DistanceCalc((APointBase *)  from, (ALineBase *)   to);   break;
      case 65536*1+4: result = DistanceCalc((APointBase *)  from, (APlaneBase *)  to);   break;
      case 65536*1+8: result = DistanceCalc((APointBase *)  from, (ASphereBase *) to);   break;
      case 65536*2+1: result = DistanceCalc((APointBase *)  to,   (ALineBase *)   from); break;
      case 65536*2+2: result = DistanceCalc((ALineBase *)   from, (ALineBase *)   to);   break;
      case 65536*2+4: result = DistanceCalc((ALineBase *)   from, (APlaneBase *)  to);   break;
      case 65536*2+8: result = DistanceCalc((ALineBase *)   from, (ASphereBase *) to);   break;
      case 65536*4+1: result = DistanceCalc((APointBase *)  to,   (APlaneBase *)  from); break;
      case 65536*4+2: result = DistanceCalc((ALineBase *)   to,   (APlaneBase *)  from); break;
      case 65536*4+4: result = DistanceCalc((APlaneBase *)  from, (APlaneBase *)  to);   break;
      case 65536*4+8: result = DistanceCalc((APlaneBase *)  from, (ASphereBase *) to);   break;
      case 65536*8+1: result = DistanceCalc((APointBase *)  to,   (ASphereBase *) from); break;
      case 65536*8+2: result = DistanceCalc((ALineBase *)   to,   (ASphereBase *) from); break;
      case 65536*8+4: result = DistanceCalc((APlaneBase *)  to,   (ASphereBase *) from); break;
      case 65536*8+8: result = DistanceCalc((ASphereBase *) from, (ASphereBase *) to);   break;
      default: break;
   }

   return result;
}

RelPosReturn GeometryCalc::RelativePosition(AGeometryBase * from, AGeometryBase * to)
{
   RelPosReturn result;

   long low  = GeometryTypeToLong(from->type);
   long high = GeometryTypeToLong(to->type);
   long mode = high << 16 | low;

   switch(mode)
   {
      case 65536*1+1: result = RelativePositionCalc((APointBase *)  from, (APointBase *)  to);   break;
      case 65536*1+2: result = RelativePositionCalc((APointBase *)  from, (ALineBase *)   to);   break;
      case 65536*1+4: result = RelativePositionCalc((APointBase *)  from, (APlaneBase *)  to);   break;
      case 65536*1+8: result = RelativePositionCalc((APointBase *)  from, (ASphereBase *) to);   break;
      case 65536*2+1: result = RelativePositionCalc((APointBase *)  to,   (ALineBase *)   from); break;
      case 65536*2+2: result = RelativePositionCalc((ALineBase *)   from, (ALineBase *)   to);   break;
      case 65536*2+4: result = RelativePositionCalc((ALineBase *)   from, (APlaneBase *)  to);   break;
      case 65536*2+8: result = RelativePositionCalc((ALineBase *)   from, (ASphereBase *) to);   break;
      case 65536*4+1: result = RelativePositionCalc((APointBase *)  to,   (APlaneBase *)  from); break;
      case 65536*4+2: result = RelativePositionCalc((ALineBase *)   to,   (APlaneBase *)  from); break;
      case 65536*4+4: result = RelativePositionCalc((APlaneBase *)  from, (APlaneBase *)  to);   break;
      case 65536*4+8: result = RelativePositionCalc((APlaneBase *)  from, (ASphereBase *) to);   break;
      case 65536*8+1: result = RelativePositionCalc((APointBase *)  to,   (ASphereBase *) from); break;
      case 65536*8+2: result = RelativePositionCalc((ALineBase *)   to,   (ASphereBase *) from); break;
      case 65536*8+4: result = RelativePositionCalc((APlaneBase *)  to,   (ASphereBase *) from); break;
      case 65536*8+8: result = RelativePositionCalc((ASphereBase *) from, (ASphereBase *) to);   break;
      default: break;
   }

   return result;
}



float GeometryCalc::DistanceCalc(APointBase *point1, APointBase *point2)
{
   if(!(MLD_PTR_CHECK(point1) && MLD_PTR_CHECK(point2))) return 0.f;

   return UKismetMathLibrary::VSize(point2->Coordinate-point1->Coordinate);
}
float GeometryCalc::DistanceCalc(APointBase *point, ALineBase *line)
{
   if(!(MLD_PTR_CHECK(point) && MLD_PTR_CHECK(line))) return 0.f;

   FVector v = point->Coordinate - line->Position;

   return UKismetMathLibrary::VSize(FVector::CrossProduct(v, line->Direction)) / UKismetMathLibrary::VSize(line->Direction);
}
float GeometryCalc::DistanceCalc(APointBase *point, APlaneBase *plane)
{
   if(!(MLD_PTR_CHECK(point) && MLD_PTR_CHECK(plane))) return 0.f;

   return 1.0f;
}
float GeometryCalc::DistanceCalc(APointBase *point, ASphereBase *sphere)
{
   if(!(MLD_PTR_CHECK(point) && MLD_PTR_CHECK(sphere))) return 0.f;

   return 2.0f;
}
float GeometryCalc::DistanceCalc(ALineBase *line1, ALineBase *line2)
{
   if(!(MLD_PTR_CHECK(line1) && MLD_PTR_CHECK(line2))) return 0.f;

   return 3.0f;
}
float GeometryCalc::DistanceCalc(ALineBase *line, APlaneBase *plane)
{
   if(!(MLD_PTR_CHECK(line) && MLD_PTR_CHECK(plane))) return 0.f;

   return 4.0f;
}
float GeometryCalc::DistanceCalc(ALineBase *line, ASphereBase *sphere)
{
   if(!(MLD_PTR_CHECK(line) && MLD_PTR_CHECK(sphere))) return 0.f;

   return 5.0f;
}
float GeometryCalc::DistanceCalc(APlaneBase *plane1, APlaneBase *plane2)
{
   if(!(MLD_PTR_CHECK(plane1) && MLD_PTR_CHECK(plane2))) return 0.f;

   return 6.0f;
}
float GeometryCalc::DistanceCalc(APlaneBase *plane, ASphereBase *sphere)
{
   if(!(MLD_PTR_CHECK(plane) && MLD_PTR_CHECK(sphere))) return 0.f;

   return 7.0f;
}
float GeometryCalc::DistanceCalc(ASphereBase *sphere1, ASphereBase *sphere2)
{
   if(!(MLD_PTR_CHECK(sphere1) && MLD_PTR_CHECK(sphere2))) return 0.f;

   return 8.0f;
}

RelPosReturn GeometryCalc::RelativePositionCalc(APointBase * point1, APointBase * point2)
{
   return RelPosReturn();
}
RelPosReturn GeometryCalc::RelativePositionCalc(APointBase * point, ALineBase * line)
{
   return RelPosReturn();
}
RelPosReturn GeometryCalc::RelativePositionCalc(APointBase * point, APlaneBase * plane)
{
   return RelPosReturn();
}
RelPosReturn GeometryCalc::RelativePositionCalc(APointBase * point, ASphereBase * sphere)
{
   return RelPosReturn();
}
RelPosReturn GeometryCalc::RelativePositionCalc(ALineBase * line1, ALineBase * line2)
{
   return RelPosReturn();
}
RelPosReturn GeometryCalc::RelativePositionCalc(ALineBase * line, APlaneBase * plane)
{
   return RelPosReturn();
}
RelPosReturn GeometryCalc::RelativePositionCalc(ALineBase * line, ASphereBase * sphere)
{
   return RelPosReturn();
}
RelPosReturn GeometryCalc::RelativePositionCalc(APlaneBase * plane1, APlaneBase * plane2)
{
   return RelPosReturn();
}
RelPosReturn GeometryCalc::RelativePositionCalc(APlaneBase * plane, ASphereBase * sphere)
{
   return RelPosReturn();
}
RelPosReturn GeometryCalc::RelativePositionCalc(ASphereBase * sphere1, ASphereBase * sphere2)
{
   return RelPosReturn();
}


#pragma once

// Forward declarations.
class AGeometryBase;
class ALineBase;
class APlaneBase;
class APointBase;
class ASphereBase;
class AUnitBase;
class AVectorStruct;




struct RelPosReturn
{
   SolutionType type;
   TArray<FVector> vectors;
};

class MATHLAB_API GeometryCalc
{

public:
    GeometryCalc();
   ~GeometryCalc();

   float        Distance(        AGeometryBase *from, AGeometryBase *to);
   RelPosReturn RelativePosition(AGeometryBase *from, AGeometryBase *to);

protected:
   float DistanceCalc( APointBase  *point1,  APointBase  *point2);
   float DistanceCalc( APointBase  *point,   ALineBase   *line);
   float DistanceCalc( APointBase  *point,   APlaneBase  *plane);
   float DistanceCalc( APointBase  *point,   ASphereBase *sphere);
   float DistanceCalc(  ALineBase  *line1,   ALineBase   *line2);
   float DistanceCalc(  ALineBase  *line,    APlaneBase  *plane);
   float DistanceCalc(  ALineBase  *line,    ASphereBase *sphere);
   float DistanceCalc( APlaneBase  *plane1,  APlaneBase  *plane2);
   float DistanceCalc( APlaneBase  *plane,   ASphereBase *sphere);
   float DistanceCalc(ASphereBase  *sphere1, ASphereBase *sphere2);

   RelPosReturn RelativePositionCalc( APointBase  *point1,  APointBase  *point2);
   RelPosReturn RelativePositionCalc( APointBase  *point,   ALineBase   *line);
   RelPosReturn RelativePositionCalc( APointBase  *point,   APlaneBase  *plane);
   RelPosReturn RelativePositionCalc( APointBase  *point,   ASphereBase *sphere);
   RelPosReturn RelativePositionCalc(  ALineBase  *line1,   ALineBase   *line2);
   RelPosReturn RelativePositionCalc(  ALineBase  *line,    APlaneBase  *plane);
   RelPosReturn RelativePositionCalc(  ALineBase  *line,    ASphereBase *sphere);
   RelPosReturn RelativePositionCalc( APlaneBase  *plane1,  APlaneBase  *plane2);
   RelPosReturn RelativePositionCalc( APlaneBase  *plane,   ASphereBase *sphere);
   RelPosReturn RelativePositionCalc(ASphereBase  *sphere1, ASphereBase *sphere2);

   long GeometryTypeToLong(GeometryType type);



};
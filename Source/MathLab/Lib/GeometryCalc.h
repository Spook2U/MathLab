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
   RelativePosition type;
   TArray<FVector> vectors;
};

class MATHLAB_API GeometryCalc
{
public:
    GeometryCalc();
   ~GeometryCalc();


   
   // public Calculation Functions

   /* Calculates the Distance between 2 Geometry Objects.*/
   float GetDistance(AGeometryBase *from, AGeometryBase *to);
   
   /* Determines the relatives position between 2 Geometry Objects.
      The struct contains the relative position and depending on the relative postion an array of interception points. */
   RelPosReturn GetRelativePosition(AGeometryBase *from, AGeometryBase *to);

   float HesseNormalFormPlugIn(APlaneBase *plane, FVector point);
   float HesseNormalFormPlugIn(APlaneBase *plane, APointBase *point);


protected:
   /* Helper function for the Calculation Functions. */
   long GeometryTypeToLong(GeometryType type);

   float VectorDistance(FVector v1, FVector v2);
   
   float DistanceCalc(APointBase  *point1,  APointBase  *point2);
   float DistanceCalc(APointBase  *point,   ALineBase   *line);
   float DistanceCalc(APointBase  *point,   APlaneBase  *plane);
   float DistanceCalc(APointBase  *point,   ASphereBase *sphere);
   float DistanceCalc(ALineBase   *line,    APointBase  *point);
   float DistanceCalc(ALineBase   *line1,   ALineBase   *line2);
   float DistanceCalc(ALineBase   *line,    APlaneBase  *plane);
   float DistanceCalc(ALineBase   *line,    ASphereBase *sphere);
   float DistanceCalc(APlaneBase  *plane,   APointBase  *point);
   float DistanceCalc(APlaneBase  *plane,   ALineBase   *line);
   float DistanceCalc(APlaneBase  *plane1,  APlaneBase  *plane2);
   float DistanceCalc(APlaneBase  *plane,   ASphereBase *sphere);
   float DistanceCalc(ASphereBase *sphere,  APointBase  *point);
   float DistanceCalc(ASphereBase *sphere,  ALineBase   *line);
   float DistanceCalc(ASphereBase *sphere,  APlaneBase  *plane);
   float DistanceCalc(ASphereBase *sphere1, ASphereBase *sphere2);

   RelPosReturn RelativePositionCalc(APointBase  *point1,  APointBase  *point2);
   RelPosReturn RelativePositionCalc(APointBase  *point,   ALineBase   *line);
   RelPosReturn RelativePositionCalc(APointBase  *point,   APlaneBase  *plane);
   RelPosReturn RelativePositionCalc(APointBase  *point,   ASphereBase *sphere);
   RelPosReturn RelativePositionCalc(ALineBase   *line,    APointBase  *point);
   RelPosReturn RelativePositionCalc(ALineBase   *line1,   ALineBase   *line2);
   RelPosReturn RelativePositionCalc(ALineBase   *line,    APlaneBase  *plane);
   RelPosReturn RelativePositionCalc(ALineBase   *line,    ASphereBase *sphere);
   RelPosReturn RelativePositionCalc(APlaneBase  *plane,   APointBase  *point);
   RelPosReturn RelativePositionCalc(APlaneBase  *plane,   ALineBase   *line);
   RelPosReturn RelativePositionCalc(APlaneBase  *plane1,  APlaneBase  *plane2);
   RelPosReturn RelativePositionCalc(APlaneBase  *plane,   ASphereBase *sphere);
   RelPosReturn RelativePositionCalc(ASphereBase *sphere,  APointBase  *point);
   RelPosReturn RelativePositionCalc(ASphereBase *sphere,  ALineBase   *line);
   RelPosReturn RelativePositionCalc(ASphereBase *sphere,  APlaneBase  *plane);
   RelPosReturn RelativePositionCalc(ASphereBase *sphere1, ASphereBase *sphere2);
};
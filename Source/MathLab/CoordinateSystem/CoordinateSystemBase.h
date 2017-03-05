// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "MathLabEnums.h"
#include "Math/LinearSystem.h"
#include "Lib/MathLabLibrary.h"

#include "CircleBase.h"
#include "LineBase.h"
#include "PlaneBase.h"
#include "PointBase.h"
#include "SphereBase.h"
#include "UnitBase.h"
#include "CVectorBase.h"

#include "CoordinateSystemBase.generated.h"

class AGeometryBase;



UCLASS()
class MATHLAB_API ACoordinateSystemBase : public AActor
{
	GENERATED_BODY()

public:	
	ACoordinateSystemBase();
   
   virtual void BeginPlay() override;
   virtual void OnConstruction(const FTransform& Transform) override;
   virtual void Tick( float DeltaSeconds ) override;

public:
// Coordinate System -------------------------------------------------------------------------------
   
   /* Length of each positive and negative side of the axis in m*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate system", meta = (ClampMin = 0.5, UIMin = 1, UIMax = 9))
   float axisLength;
   /* Diameter of the Axis and Unit in m*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate system", meta = (ClampMin = 0.01, UIMin = 1))
   float axisSize;

   /* Glow intensity of the laser of the Units*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "coordinate system", meta = (ClampMin = 0))
   float glowiness;

// Coordinate System - Units------------------------------------------------------------------------

   /* Number of Units shown on each positive and negative side of each axis*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate system|unit", meta = (ClampMin = 1, UIMin = 1, UIMax = 50))
   int unitCount;
   /* Color of the laser grid from the Units*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate system|unit")
   LaserColors unitLaserColor;
   /* Glow intensity of the laser of the Units*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "coordinate system", meta = (ClampMin = 0))
   float unitGlowiness;
   /* Size in percent from the AxisSize (diameter)*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate system|unit", meta = (ClampMin = 0.01, UIMin = 0.01, UIMax = 1))
   float unitSizeFactor;
   /* Size in percent from the AxisSize (diameter)*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate system|unit", meta = (ClampMin = 0.01, UIMin = 0.01, UIMax = 1))
   float unitLaserSizeFactor;

// Text --------------------------------------------------------------------------------------------

   /* Tougles if names from objects should be shown by default. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "text")
   bool showNames;
   /* Toughles if mathematical data should be shown by default. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "text")
   bool showMathData;
   /* Determines the size of the text of all geometry objects. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "text", meta = (ClampMin = 0.01, UIMin = 2, UIMax = 20))
   float nameTextSize;
   /* determines the size if the unit text. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "text", meta = (ClampMin = 0.01, UIMin = 2, UIMax = 20))
   float unitTextSize;

   /* Toughles if names from the constructing vectors should be shown by default. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "text")
   bool showCVectorName;
   /* Toughles if mathematical data from the constructing vectors should be shown by default. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "text")
   bool showCVectorMathData;
   /* determines the size if the constructing vector text. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "text", meta = (ClampMin = 0.01, UIMin = 2, UIMax = 20))
   float cVectorTextSize;
   
   // Geht das? Wie greife ich auf die ScalarParameter eines TextRenders zu?   
   ///* Glow intensity of the texts. */
   //UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "text", meta = (ClampMin = 0))
   //float textGlowiness;

// No Settings -------------------------------------------------------------------------------------

   /* Saves all Objects created in the CoordinateSystem*/
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "coordinate system") 
   TArray<AGeometryBase *> elements;
   /* Used to convert from Coordinate to Location*/
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "coordinate system")
   float convertFactor;
   /* Defines the biggest Coordinate still visible*/
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "coordinate system")
   float maxCoordinate;

   int circleCounter;
   int lineCounter;
   int planeCounter;
   int pointCounter;
   int sphereCounter;
   int unitCounter;
   int constVectorCounter;
   int geometryCounter;


private:
   UStaticMeshComponent *xAxis;
   UStaticMeshComponent *yAxis;
   UStaticMeshComponent *zAxis;

   TSubclassOf<AGeometryBase> circleBP;
   TSubclassOf<AGeometryBase> lineBP;
   TSubclassOf<AGeometryBase> planeBP;
   TSubclassOf<AGeometryBase> pointBP;
   TSubclassOf<AGeometryBase> sphereBP;
   TSubclassOf<AGeometryBase> unitBP;
   TSubclassOf<AGeometryBase> cVectorBP;

   //Temp Object, cause static Library not possible with Unreal v4.14 (Known Bug)
   MathLabLibrary m;

// -------------------------------------------------------------------------------------------------

public:
   //Test Function to easily test Code
   UFUNCTION(BlueprintCallable, Category = "Test")
   void TestFunction();

   UFUNCTION(BlueprintPure, Category = "math lab|coordinate system")
   float MaxVisibleLength();

   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate system")
   void SetComponents(UStaticMeshComponent *inXAxis, UStaticMeshComponent *inYAxis, UStaticMeshComponent *inZAxis);


   //Updates the Coordinate System and all of its geometry
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate system")
   void Update();

   //Changes size of the axis and updates the ConvertFactor
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate system")
   void ScaleAxis(float length, float diameter);



   AGeometryBase * AddGeometry(TSubclassOf<AGeometryBase> geometry);
   
   //Spawns Unit Objects at the Coordinate System
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|add")
   void AddUnits();
   
   /* Spawns a Point in the Coordinate System*/
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|add")
   APointBase *AddPoint(LaserColors color, FMathPoint inPoint, FString inName = "", bool showGuides = false);
   
   /* Spawns a Line with the defines mode
      Mode line: Spawns an endless line using position and direction Vector.
      Mode segment: Spawns a segmented line between 2 points A (position) and B (direction).
      Mode vector: Spawns a vector pointing to direction, using position as offset from the orign.*/
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|add")
   ALineBase *AddLine(LaserColors color, FMathLine inLine, LineMode mode, FString inName = "", bool showGuides = false);
   
   /* Spawns a Plane with the defines mode
      Mode plane: Spawns an endless plane using position and 2 direction Vector.*/
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|add")
   APlaneBase *AddPlane(LaserColors color, FMathPlane inPlane, PlaneMode mode, FString inName = "", bool showGuides = false);
   
   /* Spawns a Sphere with the given values.*/
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|add")
   ASphereBase *AddSphere(LaserColors color, FMathSphere inSphere, FString inName = "", bool showGuides = false);

   /* Spawns a Circle on a plane with the efined values.*/
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|add")
   ACircleBase *AddCircle(LaserColors color, FMathCircle inCircle, FString inName = "", bool showGuides = false);

   ACVectorBase *AddCVector(LaserColors color, FVector pointA, FVector pointB, CVectorMode mode = CVectorMode::general, FString inName = "");

   /* Searches for the geometry with the selected name. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|find")
   AGeometryBase *FindGeometryWithName(FString inName, bool &found);
   /* Searches alles circle with the selected data. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|find")
   TArray<ACircleBase *> FindCircle(FMathCircle inCircle, bool &found);
   /* Searches alles lies with the selected data. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|find")
   TArray<ALineBase *>   FindLine(FMathLine inLine, bool &found);
   /* Searches alles planes with the selected data. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|find")
   TArray<APlaneBase *>  FindPlane(FMathPlane inPlane, bool &found);
   /* Searches alles points with the selected data. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|find")
   TArray<APointBase *>  FindPoint(FMathPoint inPoint, bool &found);
   /* Searches alles sphere with the selected data. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|find")
   TArray<ASphereBase *> FindSphere(FMathSphere inSphere, bool &found);

   /* Removes the target geometry from the coordinatesystem. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|remove")
   void Remove(AGeometryBase *targetGeometry);

   //Checks if the name is already in use
   bool NameNotUsed(FString inName);



// -------------------------------------------------------------------------------------------------
// Cannt use static function in BlueprintFunctionLibrary Workaround
// ToDo: Move functions into MathLabLibraray and make static

   /* This method is only working in Debug or Development Mode */
   UFUNCTION(BlueprintCallable, DisplayName = "Print (Debug)", Category = "math lab|string", meta = (Keywords = "mld, print, debug, printdebug"))
   static void bp_debug_Screen(FString inString = "Hello", FLinearColor color = FLinearColor::White);

   UFUNCTION(BlueprintCallable, Category = "Math Lab|Linear Equalation")
   void LE_Solve(FLinearSystem inLinearEqualation);

   // Autocast Function
   
   UFUNCTION(BlueprintPure, DisplayName = "ToString (nVector)", Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FNVectorToString(FNVector inNVector);
   UFUNCTION(BlueprintPure, DisplayName = "ToString (nMatrix)", Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FNMatrixToString(FNMatrix inNMatrix);
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Linear System)", Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FLinearSystemToString(FLinearSystem inSystem);
   
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Math Circle)", Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FMathCircleToString(FMathCircle inCircle);
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Math Line)", Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FMathLineToString(FMathLine inLine);
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Math Plane)", Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FMathPlaneToString(FMathPlane inPlane);
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Math Point)", Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FMathPointToString(FMathPoint inPoint);
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Math Sphere)", Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FMathSphereToString(FMathSphere inSphere);
   
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Circle Base)", Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString BPCircleToString(ACircleBase *inCircle);
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Line Base)", Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString BPLineToString(ALineBase *inLine);
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Plane Base)", Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString BPPlaneToString(APlaneBase *inPlane);
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Point Base)", Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString BPPointToString(APointBase *inPoint);
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Sphere Base)", Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString BPSphereToString(ASphereBase *inSphere);
   
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Intersection)", Category = "math lab|struct", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString IntersectionToString(FIntersection inIntersection);
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Relative Position)", Category = "math lab|struct", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString RelativePositionToString(FRelativePosition inRelativePosition);

   UFUNCTION(BlueprintPure, DisplayName = "ToCircleStruct (Circle Base)", Category = "math lab|struct", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FMathCircle CircleBPToFCircle(ACircleBase *inCircle);
   UFUNCTION(BlueprintPure, DisplayName = "ToLineStruct (Line Base)", Category = "math lab|struct", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FMathLine LineBPToFLine(ALineBase *inLine);
   UFUNCTION(BlueprintPure, DisplayName = "ToPlaneStruct (Plane Base)", Category = "math lab|struct", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FMathPlane PlaneBPToFPlane(APlaneBase *inPlane);
   UFUNCTION(BlueprintPure, DisplayName = "ToPointStruct (Point Base)", Category = "math lab|struct", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FMathPoint PointBPToFPoint(APointBase *inPoint);
   UFUNCTION(BlueprintPure, DisplayName = "ToSphereStruct (Sphere Base)", Category = "math lab|struct", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FMathSphere SphereBPToFSphere(ASphereBase *inSphere);


   /* Returns the distance from one geometry to the other.*/
   UFUNCTION(BlueprintCallable, Category = "math lab|calculate")
   float Distance(AGeometryBase *from, AGeometryBase *to);

   /* Returns the relative Positio from one geometry to the other.*/
   UFUNCTION(BlueprintCallable, Category = "math lab|calculate", DisplayName = "Relative Position")
   FRelativePosition GetRelativePosition(AGeometryBase *from, AGeometryBase *to);



// -------------------------------------------------------------------------------------------------


private:
   //Sub function for AddUnits()
   void AddUnits_ToAxis(UStaticMeshComponent *axis, int index);
};



// Extern Access for Actor to call EndPlay() in DebugTools
#ifdef _UE_BUILD_DEBUG_FLAG_
extern ACoordinateSystemBase *g_this;
#endif


/**Custom Event, to override a c++ function in Blueprint*/
//UFUNCTION(BlueprintNativeEvent, Category = "coordinate System|Init")
//void InitCoordinateSystem();
//void InitCoordinateSystem_Implementation(); // needs Body in cpp file

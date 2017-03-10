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
// Coordinate System ---------------------------------------------------------------------------------------------------------------------------------
   
   /* Length of each positive and negative side of the axis in m. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate system", meta = (ClampMin = 0.5, UIMin = 1, UIMax = 9))
   float axisLength;
   /* Diameter of the axis and unit in m. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate system", meta = (ClampMin = 0.01, UIMin = 1))
   float axisSize;

   /* Glow intensity of the laser of the units. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "coordinate system", meta = (ClampMin = 0))
   float glowiness;

// Coordinate System - Units--------------------------------------------------------------------------------------------------------------------------

   /* Determines if the laser of the units should be shown. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate system|unit")
   bool showUnitLaser;
   /* Number of units shown on each positive and negative side of each axis. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate system|unit", meta = (ClampMin = 1, UIMin = 1, UIMax = 50))
   int unitCount;
   /* Color of the laser grid from the units. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate system|unit")
   LaserColors unitLaserColor;
   /* Glow intensity of the laser of the units. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "coordinate system|unit", meta = (ClampMin = 0))
   float unitGlowiness;
   /* Size in percent from the axisSize (diameter). */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate system|unit", meta = (ClampMin = 0.01, UIMin = 0.01, UIMax = 1))
   float unitSizeFactor;
   /* Size in percent from the axisSize (diameter). */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate system|unit", meta = (ClampMin = 0.01, UIMin = 0.01, UIMax = 1))
   float unitLaserSizeFactor;

// Text ----------------------------------------------------------------------------------------------------------------------------------------------

   /* Tougles if names from objects should be shown by default. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "text")
   bool showNames;
   /* Toughles if mathematical data should be shown by default. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "text")
   bool showMathData;
   /* Determines the size of the text of all geometry objects. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "text", meta = (ClampMin = 0.01, UIMin = 2, UIMax = 20))
   float nameTextSize;
   /* Determines the size if the unit text. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "text", meta = (ClampMin = 0.01, UIMin = 2, UIMax = 20))
   float unitTextSize;

   /* Toughles if names from the constructing vectors should be shown by default. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "text")
   bool showCVectorName;
   /* Toughles if mathematical data from the constructing vectors should be shown by default. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "text")
   bool showCVectorMathData;
   /* Determines the size if the constructing vector text. */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "text", meta = (ClampMin = 0.01, UIMin = 2, UIMax = 20))
   float cVectorTextSize;
   
   // Geht das? Wie greife ich auf die ScalarParameter eines TextRenders zu?   
   ///* Glow intensity of the texts. */
   //UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "text", meta = (ClampMin = 0))
   //float textGlowiness;

// Member --------------------------------------------------------------------------------------------------------------------------------------------

   /* Holds all objects created within the coordinatesystem. */
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "coordinate system") 
   TArray<AGeometryBase *> elements;
   /* Used to convert from coordinate to location. */
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "coordinate system")
   float convertFactor;
   /* Defines the biggest visible coordinate. */
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "coordinate system")
   float maxCoordinate;

   // Counter for the default names.
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

   //Temp Object, cause static Library is buggy (Known Bug, Fix at Version 4.16)
   MathLabLibrary m;

// ---------------------------------------------------------------------------------------------------------------------------------------------------

public:
// Test Function -------------------------------------------------------------------------------------------------------------------------------------

   /* Test function to easily test code. */
   UFUNCTION(BlueprintCallable, Category = "Test")
   void TestFunction();

// Coordinate System Setup ---------------------------------------------------------------------------------------------------------------------------

   /* Sets the staticMeshReferences in the source file to work on. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate system")
   void SetComponents(UStaticMeshComponent *inXAxis, UStaticMeshComponent *inYAxis, UStaticMeshComponent *inZAxis);

   /* Changes size of the axis and updates the convertFactor. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate system")
   void ScaleAxis(float length, float diameter);

   /* Updates the coordinatesystem and all of its geometry. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate system")
   void Update();

// Add Functions--------------------------------------------------------------------------------------------------------------------------------------

   AGeometryBase *AddGeometry(TSubclassOf<AGeometryBase> geometry, AActor *parent);
   
   /* Spawns unit objects in the coordinatesystem. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|add")
   void AddUnits();
   
   /* Spawns a point in the coordinatesystem. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|add")
   APointBase *AddPoint(LaserColors color, FMathPoint inPoint, FName inName = "", bool showGuides = false);
   
   /* Spawns a line in the defined mode in the coordinatesystem.
        * line: Spawns an endless line using position and direction vector.
        * segment: Spawns a segmented line between 2 points A (position) and B (direction).
        * vector: Spawns a vector pointing to direction, using position as offset from the orign. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|add")
   ALineBase *AddLine(LaserColors color, FMathLine inLine, LineMode mode, FName inName = "", bool showGuides = false);
   
   /* Spawns a Plane in the defines mode in the coordinatsystem.
      Mode plane: Spawns an endless plane using position and 2 direction vector. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|add")
   APlaneBase *AddPlane(LaserColors color, FMathPlane inPlane, PlaneMode mode, FName inName = "", bool showGuides = false);
   
   /* Spawns a sphere in the coordinatesystem. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|add")
   ASphereBase *AddSphere(LaserColors color, FMathSphere inSphere, FName inName = "", bool showGuides = false);

   /* Spawns a circle inside a plane in the coordinatesystem. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|add")
   ACircleBase *AddCircle(LaserColors color, FMathCircle inCircle, FName inName = "", bool showGuides = false);

   /* Spawns a constructing vector. */
   ACVectorBase *AddCVector(AActor *parent, LaserColors color, FVector pointA, FVector pointB, CVectorMode mode = CVectorMode::general, FName inName = "");

// Find Functions-------------------------------------------------------------------------------------------------------------------------------------

   /* Searches for the geometry with the selected name. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|find")
   AGeometryBase *FindGeometryWithName(FName inName, bool &found);
   /* Searches all circle with the selected data. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|find")
   TArray<ACircleBase *> FindCircle(FMathCircle inCircle, bool &found);
   /* Searches all lines with the selected data. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|find")
   TArray<ALineBase *>   FindLine(FMathLine inLine, bool &found);
   /* Searches all planes with the selected data. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|find")
   TArray<APlaneBase *>  FindPlane(FMathPlane inPlane, bool &found);
   /* Searches all points with the selected data. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|find")
   TArray<APointBase *>  FindPoint(FMathPoint inPoint, bool &found);
   /* Searches all sphere with the selected data. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|find")
   TArray<ASphereBase *> FindSphere(FMathSphere inSphere, bool &found);

// Remove Functions-----------------------------------------------------------------------------------------------------------------------------------

   /* Removes the target geometry from the coordinatesystem. */
   UFUNCTION(BlueprintCallable, Category = "math lab|coordinate System|remove")
   void Remove(AGeometryBase *targetGeometry);

// Utility Functions----------------------------------------------------------------------------------------------------------------------------------

   /* Returns the maximum length of a segemnt that can be visible in th coordinatesystem. */
   UFUNCTION(BlueprintPure, Category = "math lab|coordinate system")
   float MaxVisibleLength();

   /* Checks if the name is already in use. */
   bool NameNotUsed(FName inName);



// ---------------------------------------------------------------------------------------------------------------------------------------------------
// Cannt use static function in BlueprintFunctionLibrary Workaround
// ToDo: Move functions into MathLabLibraray and make static
// ---------------------------------------------------------------------------------------------------------------------------------------------------

   /* This method is only working in Debug or Development Mode. */
   UFUNCTION(BlueprintCallable, DisplayName = "Print (Debug)", Category = "math lab|string", meta = (Keywords = "mld, print, debug, printdebug"))
   static void bp_debug_Screen(FString inString = "Hello", FLinearColor color = FLinearColor::White);

   /* Solves the give linear system. */
   UFUNCTION(BlueprintCallable, Category = "Math Lab|Linear System")
   void SolveLinearSystem(FLinearSystem inLinearSystem);

// Autocast Function ---------------------------------------------------------------------------------------------------------------------------------
   
   /* Converts a nVector value to a string. */
   UFUNCTION(BlueprintPure, DisplayName = "ToString (nVector)",           Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FNVectorToString(FNVector inNVector);                          
   /* Converts a nMatrix value to a string. */                            
   UFUNCTION(BlueprintPure, DisplayName = "ToString (nMatrix)",           Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FNMatrixToString(FNMatrix inNMatrix);                          
   /* Converts a LinearSystem value to a string. */                       
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Linear System)",     Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FLinearSystemToString(FLinearSystem inSystem);                 
                                                                          
   /* Converts a MathCircle value to a string. */                         
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Math Circle)",       Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FMathCircleToString(FMathCircle inCircle);                     
   /* Converts a MathLine value to a string. */                           
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Math Line)",         Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FMathLineToString(FMathLine inLine);                           
   /* Converts a MathPlane value to a string. */                          
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Math Plane)",        Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FMathPlaneToString(FMathPlane inPlane);                        
   /* Converts a MathPoint value to a string. */                          
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Math Point)",        Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FMathPointToString(FMathPoint inPoint);                        
   /* Converts a MathSphere value to a string. */                         
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Math Sphere)",       Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FMathSphereToString(FMathSphere inSphere);                     
                                                                          
   /* Converts a Circle value to a string. */                             
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Circle Base)",       Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString BPCircleToString(ACircleBase *inCircle);                       
   /* Converts a Line value to a string. */                               
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Line Base)",         Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString BPLineToString(ALineBase *inLine);                             
   /* Converts a Plane value to a string. */                              
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Plane Base)",        Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString BPPlaneToString(APlaneBase *inPlane);                          
   /* Converts a Point value to a string. */                              
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Point Base)",        Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString BPPointToString(APointBase *inPoint);                          
   /* Converts a Sphere value to a string. */                             
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Sphere Base)",       Category = "math lab|string", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString BPSphereToString(ASphereBase *inSphere);
   
   /* Converts a Intersection value to a string. */
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Intersection)",      Category = "math lab|struct", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString IntersectionToString(FIntersection inIntersection);
   /* Converts a RelativePosition value to a string. */
   UFUNCTION(BlueprintPure, DisplayName = "ToString (Relative Position)", Category = "math lab|struct", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString RelativePositionToString(FRelativePosition inRelativePosition);

   /* Converts a Circle value to a MathCircle. */
   UFUNCTION(BlueprintPure, DisplayName = "ToCircleStruct (Circle Base)", Category = "math lab|struct", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FMathCircle CircleBPToFCircle(ACircleBase *inCircle);
   /* Converts a Line value to a MathLine. */
   UFUNCTION(BlueprintPure, DisplayName = "ToLineStruct (Line Base)",     Category = "math lab|struct", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FMathLine LineBPToFLine(ALineBase *inLine);
   /* Converts a Plane value to a MathPlane. */
   UFUNCTION(BlueprintPure, DisplayName = "ToPlaneStruct (Plane Base)",   Category = "math lab|struct", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FMathPlane PlaneBPToFPlane(APlaneBase *inPlane);
   /* Converts a Point value to a MathPoint. */
   UFUNCTION(BlueprintPure, DisplayName = "ToPointStruct (Point Base)",   Category = "math lab|struct", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FMathPoint PointBPToFPoint(APointBase *inPoint);
   /* Converts a Sphere value to a MathSphere. */
   UFUNCTION(BlueprintPure, DisplayName = "ToSphereStruct (Sphere Base)", Category = "math lab|struct", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FMathSphere SphereBPToFSphere(ASphereBase *inSphere);

// Calculations --------------------------------------------------------------------------------------------------------------------------------------

   /* Returns the distance from one geometry to the other.*/
   UFUNCTION(BlueprintCallable, Category = "math lab|calculate")
   float Distance(AGeometryBase *from, AGeometryBase *to);

   /* returns the distance from the center of two spheres. */
   UFUNCTION(BlueprintCallable, Category = "math lab|calculate")
   float DistanceSphereCenter(ASphereBase *from, ASphereBase *to);

   /* Returns the relative position from one geometry to the other.*/
   UFUNCTION(BlueprintCallable, Category = "math lab|calculate", DisplayName = "Relative Position")
   FRelativePosition GetRelativePosition(AGeometryBase *from, AGeometryBase *to);



// ---------------------------------------------------------------------------------------------------------------------------------------------------

private:
   // Sub function for AddUnits()
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

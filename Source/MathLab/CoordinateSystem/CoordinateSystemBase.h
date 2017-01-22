// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MathLabTools.h"
#include "GameFramework/Actor.h"
#include "Math/LinearEqualation.h"
#include "CoordinateSystemBase.generated.h"

class AGeometryBase;
class ALineBase;
class APlaneBase;
class APointBase;
class ASphereBase;
class AUnitBase;



UCLASS()
class MATHLAB_API ACoordinateSystemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ACoordinateSystemBase();
   
// Unreal Callbacks --------------------------------------------------------------------------------
   virtual void BeginPlay() override;
   virtual void OnConstruction(const FTransform& Transform) override;
   virtual void Tick( float DeltaSeconds ) override;



// Test Debug Print Function -----------------------------------------------------------------------
   UFUNCTION(BlueprintCallable, Displayname = "Print (DebugOnly)", Category = "string", meta = (Keywords = "mld, print, debug, printdebug", Tooltip = "This method is only working in Debug or Development Mode"))
   void bp_debug_Screen(FString inString = "Hello", FLinearColor color = FLinearColor::White);



////////////////////////////////////////////////////////////////////////////////////////////////////
// Member Variable  ////////////////////////////////////////////////////////////////////////////////
public:
   //Length of each positive and negative side of the axis in m
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate System", meta = (ClampMin = 0.5, UIMin = 1, UIMax = 9))
   float AxisLength;

   //Diameter of the Axis and Unit in m
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate System", meta = (ClampMin = 0.01, UIMin = 1))
   float AxisSize;

   //Number of Units shown on each positive and negative side of each axis
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate System", meta = (ClampMin = 1, UIMin = 1, UIMax = 50))
   int UnitCount;

   //Color of the laser grid from the Units
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate System")
   LaserColors LaserColor;

// -------------------------------------------------------------------------------------------------

   //Size in percent from the AxisSize (diameter)
   UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "coordinate System", meta = (ClampMin = 0.01, UIMin = 0.01, UIMax = 1))
   float UnitSizeFactor;

   //Size in percent from the AxisSize (diameter)
   UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "coordinate System", meta = (ClampMin = 0.01, UIMin = 0.01, UIMax = 1))
   float LaserSizeFactor;

   //Glowi intensity of the laser of the Units
   UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "coordinate System", meta = (ClampMin = 0))
   float Glowiness;

// -------------------------------------------------------------------------------------------------

   //Saves all Objects created in the CoordinateSystem
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "coordinate System") 
   TArray<AGeometryBase *> Elements;

   //Used to convert from Coordinate to Location
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "coordinate System")
   float ConvertFactor;

   //Defines the biggest Coordinate still visible
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "coordinate System")
   float MaxCoordinate;

   UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "coordinate System")
   AGeometryBase *gb;


// -------------------------------------------------------------------------------------------------
private:
   UStaticMeshComponent *XAxis;
   UStaticMeshComponent *YAxis;
   UStaticMeshComponent *ZAxis;

   TSubclassOf<AGeometryBase> PointBP;
   TSubclassOf<AGeometryBase> UnitBP;
   TSubclassOf<AGeometryBase> LineBP;
   TSubclassOf<AGeometryBase> PlaneBP;
   TSubclassOf<AGeometryBase> SphereBP;

   
////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions  //////////////////////////////////////////////////////////////////////////////////////
public:
// -------------------------------------------------------------------------------------------------

   //Test Function to easily test Code
   UFUNCTION(BlueprintCallable, Category = "Test")
   void TestFunction();

// -------------------------------------------------------------------------------------------------

   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "coordinate System|Init")
   void SetComponents(UStaticMeshComponent *xAxis, UStaticMeshComponent *yAxis, UStaticMeshComponent *zAxis);

// Pure Functions -----------------------------------------------------------------------------------
   
   UFUNCTION(BlueprintPure, Category = "coordinate System|Util")
   float MaxVisibleLength();

// Update -------------------------------------------------------------------------------------------

   //Updates the Coordinate System and all of its geometry
   UFUNCTION(BlueprintCallable, Category = "coordinate System|Update")
   void Update();

// Setup --------------------------------------------------------------------------------------------

   //Changes size of the axis and updates the ConvertFactor
   UFUNCTION(BlueprintCallable, Category = "coordinate System|Setup")
   void ScaleAxis(float length, float diameter);

// Make ---------------------------------------------------------------------------------------------

   AGeometryBase * AddGeometry(bool isGuide, TSubclassOf<AGeometryBase> geometry);

   //Spawns Unit Objects at the Coordinate System
   UFUNCTION(BlueprintCallable, Category = "coordinate System|Make")
   void AddUnits();

   /**Spawns a Point at the given Coordinate in the Coordinate System*/
   UFUNCTION(BlueprintCallable, Category = "coordinate System|Make")
   APointBase *MakePoint(LaserColors color, FVector coordinate, bool showGuides);

   /**
      Spawns a Line with the defines mode
      Mode line: Spawns an endless line using position and direction Vector.
      Mode segment: Spawns a segmented line between 2 points A (position) and B (direction).
      Mode vector: Spawns a vector pointing to direction, using position as offset from the orign.
   */
   UFUNCTION(BlueprintCallable, Category = "coordinate System|Make")
   ALineBase *MakeLine(LaserColors color, FVector position, FVector direction, LineMode mode, bool showGuides);

   /**
      Spawns a Plane with the defines mode
      Mode plane: Spawns an endless plane using position and 2 direction Vector.
   */
   UFUNCTION(BlueprintCallable, Category = "coordinate System|Make")
   APlaneBase *MakePlane(LaserColors color, FVector position, FVector direction1, FVector direction2, PlaneMode mode, bool showGuides);

   /**Spawns a Sphere at the given Coordinate with the defined radius*/
   UFUNCTION(BlueprintCallable, Category = "coordinate System|Make")
   ASphereBase *MakeSphere(LaserColors color, FVector position, float radius, bool showGuides);


// -------------------------------------------------------------------------------------------------

   APointBase *AddPoint(LaserColors color, bool isGuide, FVector coordinate);
   ALineBase *AddLine(LaserColors color, bool isGuide, FVector position, FVector direction, LineMode mode);
   APlaneBase *AddPlane(LaserColors color, bool isGuide, FVector position, FVector direction1, FVector direction2, PlaneMode mode);
   ASphereBase *AddSphere(LaserColors color, bool isGuide, FVector position, float radius);


// -------------------------------------------------------------------------------------------------
private:
   //Sub function for AddUnits()
   void AddUnits_ToAxis(UStaticMeshComponent *axis, int index);
};



// Extern Access for Actor to call EndPlay() in DebugTools
#ifdef _UE_BUILD_DEBUG_FLAG_
extern ACoordinateSystemBase *g_this;
#endif


////Custom Event, to override a c++ function in Blueprint
//UFUNCTION(BlueprintNativeEvent, Category = "coordinate System|Init")
//void InitCoordinateSystem();
//void InitCoordinateSystem_Implementation(); // needs Body in cpp file

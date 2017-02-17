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
#include "VectorStruct.h"

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
   /* Length of each positive and negative side of the axis in m*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate System", meta = (ClampMin = 0.5, UIMin = 1, UIMax = 9))
   float AxisLength;
   /* Diameter of the Axis and Unit in m*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate System", meta = (ClampMin = 0.01, UIMin = 1))
   float AxisSize;
   /* Number of Units shown on each positive and negative side of each axis*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate System", meta = (ClampMin = 1, UIMin = 1, UIMax = 50))
   int UnitCount;
   /* Color of the laser grid from the Units*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category = "coordinate System")
   LaserColors LaserColor;

   /* Size in percent from the AxisSize (diameter)*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "coordinate System", meta = (ClampMin = 0.01, UIMin = 0.01, UIMax = 1))
   float UnitSizeFactor;
   /* Size in percent from the AxisSize (diameter)*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "coordinate System", meta = (ClampMin = 0.01, UIMin = 0.01, UIMax = 1))
   float LaserSizeFactor;
   /* Glowi intensity of the laser of the Units*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "coordinate System", meta = (ClampMin = 0))
   float Glowiness;

   /* Saves all Objects created in the CoordinateSystem*/
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "coordinate System") 
   TArray<AGeometryBase *> Elements;
   /* Used to convert from Coordinate to Location*/
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "coordinate System")
   float ConvertFactor;
   /* Defines the biggest Coordinate still visible*/
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "coordinate System")
   float MaxCoordinate;




private:
   UStaticMeshComponent *XAxis;
   UStaticMeshComponent *YAxis;
   UStaticMeshComponent *ZAxis;

   TSubclassOf<AGeometryBase> CircleBP;
   TSubclassOf<AGeometryBase> LineBP;
   TSubclassOf<AGeometryBase> PlaneBP;
   TSubclassOf<AGeometryBase> PointBP;
   TSubclassOf<AGeometryBase> SphereBP;
   TSubclassOf<AGeometryBase> UnitBP;
   TSubclassOf<AGeometryBase> VectorStructBP;

   //Temp Object, cause static Library not possible with Unreal v4.14 (Known Bug)
   MathLabLibrary m;

// -------------------------------------------------------------------------------------------------

public:
   //Test Function to easily test Code
   UFUNCTION(BlueprintCallable, Category = "Test")
   void TestFunction();

   UFUNCTION(BlueprintPure, Category = "coordinate System|Util")
   float MaxVisibleLength();

   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "coordinate System|Init")
   void SetComponents(UStaticMeshComponent *xAxis, UStaticMeshComponent *yAxis, UStaticMeshComponent *zAxis);


   //Updates the Coordinate System and all of its geometry
   UFUNCTION(BlueprintCallable, Category = "coordinate System|Update")
   void Update();

   //Changes size of the axis and updates the ConvertFactor
   UFUNCTION(BlueprintCallable, Category = "coordinate System|Setup")
   void ScaleAxis(float length, float diameter);



   AGeometryBase * AddGeometry(TSubclassOf<AGeometryBase> geometry);
   
   //Spawns Unit Objects at the Coordinate System
   UFUNCTION(BlueprintCallable, Category = "coordinate System|Make")
   void AddUnits();
   
   /* Spawns a Point in the Coordinate System*/
   UFUNCTION(BlueprintCallable, Category = "coordinate System|Make")
   APointBase *AddPoint(LaserColors color, FMathPoint inPoint, bool showGuides);
   
   /* Spawns a Line with the defines mode
      Mode line: Spawns an endless line using position and direction Vector.
      Mode segment: Spawns a segmented line between 2 points A (position) and B (direction).
      Mode vector: Spawns a vector pointing to direction, using position as offset from the orign.*/
   UFUNCTION(BlueprintCallable, Category = "coordinate System|Make")
   ALineBase *AddLine(LaserColors color, FMathLine inLine, LineMode mode, bool showGuides);
   
   /* Spawns a Plane with the defines mode
      Mode plane: Spawns an endless plane using position and 2 direction Vector.*/
   UFUNCTION(BlueprintCallable, Category = "coordinate System|Make")
   APlaneBase *AddPlane(LaserColors color, FMathPlane inPlane, PlaneMode mode, bool showGuides);
   
   /* Spawns a Sphere with the given values.*/
   UFUNCTION(BlueprintCallable, Category = "coordinate System|Make")
   ASphereBase *AddSphere(LaserColors color, FMathSphere inSphere, bool showGuides);

   /* Spawns a Circle on a plane with the efined values.*/
   UFUNCTION(BlueprintCallable, Category = "coordinate System|Make")
   ACircleBase *AddCircle(LaserColors color, FMathCircle inCircle, bool showGuides);

   AVectorStruct *AddVectorStruct(LaserColors color, FVector pointA, FVector pointB, VectorStructMode mode = VectorStructMode::general);



// -------------------------------------------------------------------------------------------------
// Cannt use static function in BlueprintFunctionLibrary Workaround
// ToDo: Move functions into MathLabLibraray and make static

   /* This method is only working in Debug or Development Mode */
   UFUNCTION(BlueprintCallable, DisplayName = "Print (Debug)", Category = "Math Lab|String", meta = (Keywords = "mld, print, debug, printdebug"))
   static void bp_debug_Screen(FString inString = "Hello", FLinearColor color = FLinearColor::White);

   UFUNCTION(BlueprintPure, DisplayName = "ToString (nVector)", Category = "Math Lab|String", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FNVectorToString(FNVector inNVector);

   UFUNCTION(BlueprintPure, DisplayName = "ToString (nMatrix)", Category = "Math Lab|String", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FNMatrixToString(FNMatrix inNMatrix);

   UFUNCTION(BlueprintPure, DisplayName = "ToString (Linear Equalation)", Category = "Math Lab|String", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FLinearEqualationToString(FLinearSystem inLinearEqualation);

   UFUNCTION(BlueprintCallable, Category = "Math Lab|Linear Equalation")
   void LE_Solve(FLinearSystem inLinearEqualation);



   UFUNCTION(BlueprintPure, DisplayName = "ToString (Point)", Category = "Math Lab|String", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FString FPointToString(FMathPoint inPoint);

   UFUNCTION(BlueprintPure, DisplayName = "ToPointBase (Point struct)", Category = "Math Lab|point", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   APointBase *FPointToPointBP(FMathPoint inPoint);

   UFUNCTION(BlueprintPure, DisplayName = "ToPointStruct (Point)", Category = "Math Lab|point", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   FMathPoint PointBPToFPoint(APointBase *inPoint);


   /* Returns the distance from one geometry to the other.*/
   UFUNCTION(BlueprintCallable, Category = "coordinate System|Calculate")
   float Distance(AGeometryBase *from, AGeometryBase *to);

   /* Returns the relative Positio from one geometry to the other.*/
   UFUNCTION(BlueprintCallable, Category = "coordinate System|Calculate", DisplayName = "Relative Position")
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

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CoordinateSystemBase.generated.h"

UENUM(BlueprintType)
enum class LaserColors : uint8
{
   blue     UMETA(DisplayName = "Blue"),
   green    UMETA(DisplayName = "Green"),
   orange   UMETA(DisplayName = "Orange"),
   purple   UMETA(DisplayName = "Purple"),
   red      UMETA(DisplayName = "Red"),
   white    UMETA(DisplayName = "White"),
   yellow   UMETA(DisplayName = "Yellow")
};

UCLASS()
class MATHLAB_API ACoordinateSystemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ACoordinateSystemBase();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Member Variable  ////////////////////////////////////////////////////////////////////////////////
public:
   //Length of each positive and negative side of the axis in m
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "settings")
   float AxisLength = 1;

   //Diameter of the Axis and Unit in m
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "settings")
   float AxisSize = 0.03f;

   //Number of Units shown on each positive and negative side of each axis
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "settings")
   int UnitCount = 10;

   //Size in percent from the AxisSize (diameter)
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "settings")
   float UnitSizeFactor = 0.5f;

   //Size in percent from the AxisSize (diameter)
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "settings")
   float LaserSizefactor = 0.5f;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "settings")
   LaserColors LaserColor = LaserColors::green;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "settings")
   float Glowiness = 10.f;

   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "coordinate System") 
   TArray<AGeometryBase *> Elements;

   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "coordinate System")
   float ConvertFactor;

   UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "coordinate System")
   float MaxCoordinate;

private:
   UStaticMeshComponent *XAxis;
   UStaticMeshComponent *YAxis;
   UStaticMeshComponent *ZAxis;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions  //////////////////////////////////////////////////////////////////////////////////////
public:
   UFUNCTION(BlueprintCallable, Category = "coordinate System")
   AGeometryBase * AddGeometry(bool isGuide, TSubclassOf<AGeometryBase> geometry);

   //Spawns Unit Objects at the Coordinate System
   UFUNCTION(BlueprintCallable, Category = "coordinate System")
   void AddUnits();

   //Help function for addUnits()
   void AddUnits_ToAxis(UStaticMeshComponent *axis, int index);

   //Changes size of the axis and updates the ConvertFactor
   UFUNCTION(BlueprintCallable, Category = "coordinate System")
   void ScaleAxis(float length, float diameter);

   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "coordinate System")
   void SetAxis(UStaticMeshComponent* xAxis, UStaticMeshComponent* yAxis, UStaticMeshComponent* zAxis);

   UFUNCTION(BlueprintCallable, Category = "Test")
   void TestFunction();
};

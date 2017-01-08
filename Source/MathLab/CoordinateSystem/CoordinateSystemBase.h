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
   yellow   UMETA(DisplayName = "Yellow"),
   white    UMETA(DisplayName = "White")
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
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
   float AxisLength = 1;

   //Diameter of the Axis and Unit in m
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
   float AxisSize = 0.03f;

   //Number of Units shown on each positive and negative side of each axis
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
   int UnitCount = 10;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
   LaserColors LaserColor;

   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Coordinate System") 
   TArray<AGeometryBase *> Elements;

   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Coordinate System")
   float ConvertFactor;

private:
   UStaticMeshComponent *XAxis;
   UStaticMeshComponent *YAxis;
   UStaticMeshComponent *ZAxis;

   FTransform tmp_transform;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions  //////////////////////////////////////////////////////////////////////////////////////
public:
   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "Coordinate System")
   void setAxis(UStaticMeshComponent* xAxis, UStaticMeshComponent* yAxis, UStaticMeshComponent* zAxis);

   //Changes size of the axis and updates the ConvertFactor
   UFUNCTION(BlueprintCallable, Category = "Coordinate System")
   void scaleAxis(float length, float diameter);

   UFUNCTION(BlueprintCallable, Category = "Coordinate System")
   AGeometryBase * addGeometry(bool isGuide, TSubclassOf<AGeometryBase> geometry);

   //Spawns Unit Objects at the Coordinate System
   UFUNCTION(BlueprintCallable, Category = "Coordinate System")
   void addUnits();

   //Help function for addUnits()
   void addUnits_ToAxis(UStaticMeshComponent *axis, int index);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/GeometryBase.h"
#include "PointBase.generated.h"

UCLASS()
class MATHLAB_API APointBase : public AGeometryBase
{
	GENERATED_BODY()

public:
   APointBase();

// Unreal Callbacks---------------------------------------------------------------------------------
   virtual void BeginPlay() override;



////////////////////////////////////////////////////////////////////////////////////////////////////
// Member Variable  ////////////////////////////////////////////////////////////////////////////////
public:
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "point")
   FVector Coordinate;



private:
   UStaticMeshComponent *Point;



////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions  //////////////////////////////////////////////////////////////////////////////////////
public:
   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "point|Init")
   void SetComponents(UStaticMeshComponent *point);

   //Called, when the objects need to update the position or other vales
   UFUNCTION(BlueprintCallable, Category = "point|Update")
   virtual void Update() override;

// -------------------------------------------------------------------------------------------------

   void SetValuesPoint(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector coordinate);



// -------------------------------------------------------------------------------------------------
protected:
   void CreateGuidesPoint(LaserColors color);	

	
	
	
};

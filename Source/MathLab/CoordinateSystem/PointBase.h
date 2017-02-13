// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/GeometryBase.h"
#include "PointBase.generated.h"



/* Defines a mathematical Point. */
USTRUCT(BlueprintType)
struct FMathPoint
{
   GENERATED_BODY()

public:      
   UPROPERTY(BlueprintReadWrite, Category = "point")
   FVector Coordinate;

   /* Default constructor (no initialization). */
   FMathPoint();
   /* Constructor to set the coordinate of the point. */
   FMathPoint(FVector inCoordinate);
};



UCLASS()
class MATHLAB_API APointBase : public AGeometryBase
{
	GENERATED_BODY()

public:
   APointBase();

   virtual void BeginPlay() override;

public:
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "point")
   FMathPoint point;

private:
   UStaticMeshComponent *pointMesh;

// -------------------------------------------------------------------------------------------------

public:
   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "coordinate System|plane|Init")
   void SetComponents(TArray<UStaticMeshComponent *> components);

   void InitPoint(ACoordinateSystemBase *coordinateSystem, LaserColors color, FMathPoint inPoint);

   //Called, when the objects need to update the position or other vales
   UFUNCTION(BlueprintCallable, Category = "coordinate System|point|Update")
   virtual void Update() override;


   
protected:
   virtual void CreateVectorGuides(LaserColors color) override;	

	
	
	
};

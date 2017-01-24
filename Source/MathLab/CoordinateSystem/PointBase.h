// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/GeometryBase.h"
#include "PointBase.generated.h"

class ALineBase;
class APlaneBase;

UCLASS()
class MATHLAB_API APointBase : public AGeometryBase
{
	GENERATED_BODY()

public:
   APointBase();

   virtual void BeginPlay() override;

public:
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "point")
   FVector Coordinate;

private:
   UStaticMeshComponent *Point;

// -------------------------------------------------------------------------------------------------

public:
   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "coordinate System|plane|Init")
   void SetComponents(TArray<UStaticMeshComponent *> components);

   void InitPoint(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector coordinate);

   //Called, when the objects need to update the position or other vales
   UFUNCTION(BlueprintCallable, Category = "coordinate System|point|Update")
   virtual void Update() override;

   
   UFUNCTION(BlueprintCallable, Category = "coordinate System|point|Calculate")
   float DistanceToLine(ALineBase *line);
   UFUNCTION(BlueprintCallable, Category = "coordinate System|point|Calculate")
   float DistanceToPoint(APointBase *point);
   UFUNCTION(BlueprintCallable, Category = "coordinate System|point|Calculate")
   float DistanceToPlane(APlaneBase *plane);
   float DistanceToSphere(ASphereBase *sphere);

protected:
   virtual void CreateVectorGuides(LaserColors color) override;	

	
	
	
};

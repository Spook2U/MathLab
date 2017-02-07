// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/GeometryBase.h"
#include "PlaneBase.generated.h"

UCLASS()
class MATHLAB_API APlaneBase : public AGeometryBase
{
	GENERATED_BODY()

public:
   APlaneBase();

   virtual void BeginPlay() override;

public:
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "plane")
   FMathPlane plane;
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "plane")
   PlaneMode Mode;

private:
   UStaticMeshComponent *PlaneMesh;

// -------------------------------------------------------------------------------------------------

public:
   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "coordinate System|plane|Init")
   void SetComponents(TArray<UStaticMeshComponent *> components);

   void InitPlane(ACoordinateSystemBase *coordinateSystem, LaserColors color, FMathPlane inPlane, PlaneMode mode);

   /* Returns the unified normal Vector of the plane. */
   UFUNCTION(BlueprintPure, Category = "coordinate System|plane")
   FVector GetNormal();

   //Called, when the objects need to update the position or other vales
   UFUNCTION(BlueprintCallable, Category = "coordinate System|plane|Update")
   virtual void Update() override;

   //Sets up the Components to display the plane in the selected mode
   UFUNCTION(BlueprintCallable, Category = "coordinate System|plane|Setup")
   void BuildPlane();

protected:
   virtual void CreateVectorGuides(LaserColors color) override;

	
	
};

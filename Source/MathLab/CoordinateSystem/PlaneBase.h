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
   FVector Position;
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "plane")
   FVector Direction1;
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "plane")
   FVector Direction2;
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "plane")
   FVector Normal;
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "plane")
   PlaneMode Mode;

private:
   UStaticMeshComponent *Plane;

// -------------------------------------------------------------------------------------------------

public:
   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "coordinate System|plane|Init")
   void SetComponents(TArray<UStaticMeshComponent *> components);

   void SetValuesPlane(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector position, FVector direction1, FVector direction2, PlaneMode mode);

   //Called, when the objects need to update the position or other vales
   UFUNCTION(BlueprintCallable, Category = "coordinate System|plane|Update")
   virtual void Update() override;

   //Sets up the Components to display the plane in the selected mode
   UFUNCTION(BlueprintCallable, Category = "coordinate System|plane|Setup")
   void BuildPlane();

protected:
   virtual void CreateVectorGuides(LaserColors color) override;

	
	
};

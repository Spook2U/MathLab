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

// Unreal Callbacks---------------------------------------------------------------------------------
   virtual void BeginPlay() override;



////////////////////////////////////////////////////////////////////////////////////////////////////
// Member Variable  ////////////////////////////////////////////////////////////////////////////////
public:



private:



////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions  //////////////////////////////////////////////////////////////////////////////////////
public:
   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "coordinate System|plane|Init")
   void SetComponents(UStaticMeshComponent *plane, TArray<UStaticMeshComponent *> laserComponents);

   // -------------------------------------------------------------------------------------------------

   //Called, when the objects need to update the position or other vales
   UFUNCTION(BlueprintCallable, Category = "coordinate System|plane|Update")
   virtual void Update() override;

   // -------------------------------------------------------------------------------------------------

   void SetValuesPlane(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector position, FVector direction1, FVector direction2);



   // -------------------------------------------------------------------------------------------------
protected:

	
	
};

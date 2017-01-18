// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/PointBase.h"
#include "SphereBase.generated.h"

UCLASS()
class MATHLAB_API ASphereBase : public APointBase
{
	GENERATED_BODY()

public:
   ASphereBase();

// Unreal Callbacks---------------------------------------------------------------------------------
   virtual void BeginPlay() override;



////////////////////////////////////////////////////////////////////////////////////////////////////
// Member Variable  ////////////////////////////////////////////////////////////////////////////////
public:
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "sphere")
   float Radius;



private:
   UStaticMeshComponent *Sphere;



////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions  //////////////////////////////////////////////////////////////////////////////////////
public:
// Initialise --------------------------------------------------------------------------------------

   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "coordinate System|sphere|Init")
   void SetComponents(UStaticMeshComponent *sphere, TArray<UStaticMeshComponent *> laserComponents);

// Update -------------------------------------------------------------------------------------------

   //Called, when the objects need to update the position or other vales
   UFUNCTION(BlueprintCallable, Category = "coordinate System|sphere|Update")
   virtual void Update() override;

// Setup --------------------------------------------------------------------------------------------

   //Sets up the Components to display the sphere with the specified radus
   UFUNCTION(BlueprintCallable, Category = "coordinate System|sphere|Setup")
   void BuildSphere();

// -------------------------------------------------------------------------------------------------

   void SetValuesSphere(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector coordinate, float radius);



// -------------------------------------------------------------------------------------------------
protected:
   virtual void CreateGuides(LaserColors color) override;

	
	
};

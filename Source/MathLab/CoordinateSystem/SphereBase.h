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

   virtual void BeginPlay() override;

public:
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "sphere")
   FMathSphere sphere;

private:
   UStaticMeshComponent *sphereMesh;

// -------------------------------------------------------------------------------------------------

public:
   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "coordinate System|sphere|Init")
   void SetComponents(TArray<UStaticMeshComponent *> components);

   void InitSphere(ACoordinateSystemBase *coordinateSystem, LaserColors color, FMathSphere inSphere);

   //Called, when the objects need to update the position or other vales
   UFUNCTION(BlueprintCallable, Category = "coordinate System|sphere|Update")
   virtual void Update() override;

   //Sets up the Components to display the sphere with the specified radus
   UFUNCTION(BlueprintCallable, Category = "coordinate System|sphere|Setup")
   void BuildSphere();

protected:
   virtual void CreateVectorGuides(LaserColors color) override;

	
	
};

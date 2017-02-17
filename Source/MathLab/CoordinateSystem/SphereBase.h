// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/PointBase.h"
#include "SphereBase.generated.h"



/* Defines a mathematical sphere. */
USTRUCT(BlueprintType)
struct FMathSphere : public FMathPoint
{
   GENERATED_BODY()

public:      
   UPROPERTY(BlueprintReadWrite, Category = "sphere")
   float radius;

};



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
   UFUNCTION(BlueprintCallable, Category = "math lab|sphere")
   void SetComponents(TArray<UStaticMeshComponent *> components);

   void InitSphere(ACoordinateSystemBase *inCoordinateSystem, LaserColors color, FMathSphere inSphere);

   //Called, when the objects need to update the position or other vales
   UFUNCTION(BlueprintCallable, Category = "math lab|sphere")
   virtual void Update() override;

   //Sets up the Components to display the sphere with the specified radus
   UFUNCTION(BlueprintCallable, Category = "math lab|sphere")
   void BuildSphere();

protected:
   virtual void CreateVectorGuides(LaserColors color) override;

	
	
};

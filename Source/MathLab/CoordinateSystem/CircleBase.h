// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/GeometryBase.h"
#include "CircleBase.generated.h"



/* Defines a circle on a plane. */
USTRUCT(BlueprintType)
struct FMathCircle
{
   GENERATED_BODY()

public:      
   UPROPERTY(BlueprintReadWrite, Category = "circle")
   FVector Center;
   UPROPERTY(BlueprintReadWrite, Category = "circle")
   FVector Normal;
   UPROPERTY(BlueprintReadWrite, Category = "circle")
   float Radius;

   /* Default constructor (no initialization). */
   FMathCircle();
   /* Constructor to set the circle laying on a plane. */
   FMathCircle(FVector inCenter, FVector inNormal, float inRadius);
};



UCLASS()
class MATHLAB_API ACircleBase : public AGeometryBase
{
	GENERATED_BODY()
	
public:
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "circle")
   FMathCircle circle;

private:
   UStaticMeshComponent *CircleMesh;
   UStaticMeshComponent *CircleMeshBorder;

// -------------------------------------------------------------------------------------------------

public:
   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "coordinate System|circle|Init")
   void SetComponents(TArray<UStaticMeshComponent *> components);

   void InitCircle(ACoordinateSystemBase *coordinateSystem, LaserColors color, FMathCircle inCircle);

   //Called, when the objects need to update the position or other vales
   UFUNCTION(BlueprintCallable, Category = "coordinate System|circle|Update")
   virtual void Update() override;

   //Sets up the Components to display the circle
   UFUNCTION(BlueprintCallable, Category = "coordinate System|circle|Setup")
   void BuildCircle();

protected:
   virtual void CreateVectorGuides(LaserColors color) override;

};

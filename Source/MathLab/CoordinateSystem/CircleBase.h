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
   FVector center;
   UPROPERTY(BlueprintReadWrite, Category = "circle")
   FVector normal;
   UPROPERTY(BlueprintReadWrite, Category = "circle")
   float radius;

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
   UStaticMeshComponent *circleMesh;
   UStaticMeshComponent *circleMeshBorder;

// -------------------------------------------------------------------------------------------------

public:
   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "math lab|circle")
   void SetComponents(TArray<UStaticMeshComponent *> components);

   void InitCircle(ACoordinateSystemBase *inCoordinateSystem, LaserColors color, FMathCircle inCircle);

   //Called, when the objects need to update the position or other vales
   UFUNCTION(BlueprintCallable, Category = "math lab|circle")
   virtual void Update() override;

   //Sets up the Components to display the circle
   UFUNCTION(BlueprintCallable, Category = "math lab|circle")
   void BuildCircle();

protected:
   virtual void CreateVectorGuides(LaserColors color) override;

};

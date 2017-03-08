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
   
   /* Copy another cicle into this one. */
   FMathCircle& operator=(const FMathCircle& Other);
   /* Check against another cicle for equality. */
   bool operator==(const FMathCircle& Other) const;
   /* Check against another cicle for inequality. */
   bool operator!=(const FMathCircle& Other) const;

   /* Get a textual representation of this Circle. */
   FString ToString();
   FString ToStringShort();
};

// -------------------------------------------------------------------------------------------------

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



public:
   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "math lab|circle")
   void SetComponents(TArray<UStaticMeshComponent *> components, UTextRenderComponent *inText);

   void Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FMathCircle inCircle, FString inName = "");

   //Called, when the objects need to update the position or other vales
   UFUNCTION(BlueprintCallable, Category = "math lab|circle")
   virtual void Update() override;

   /* Sets the MathCircle struct. */
   UFUNCTION(BlueprintCallable, Category = "math lab|circle")
   ACircleBase *SetCircle(FMathCircle inCircle);

   //Sets up the Components to display the circle
   UFUNCTION(BlueprintCallable, Category = "math lab|circle")
   void BuildCircle();

   UFUNCTION(Blueprintcallable, Category = "math lab|circle")
   virtual FString ToString() override;

protected:
   virtual void CreateCVector(LaserColors inColor) override;

};

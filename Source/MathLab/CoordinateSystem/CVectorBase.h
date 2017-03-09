// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/GeometryBase.h"
#include "CVectorBase.generated.h"


// Constructing Vector Mode --------------------------------------------------------------------------------------------------------------------------


UENUM(BlueprintType)
enum class CVectorMode : uint8
{
   point        UMETA(DisplayName = "Point"),
   segment      UMETA(DisplayName = "Segment"),
   vector       UMETA(DisplayName = "Vector"),
   vectorPoint  UMETA(DisplayName = "Vector to Point"),
   general      UMETA(DisplayName = "General")
};


// Constructing Vector Class -------------------------------------------------------------------------------------------------------------------------


UCLASS()
class MATHLAB_API ACVectorBase : public AGeometryBase
{
	GENERATED_BODY()
	
public:
   ACVectorBase();

public:
// Member --------------------------------------------------------------------------------------------------------------------------------------------

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "constructingVector")
   FVector a;
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "constructingVector")
   FVector b;
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "constructingVector")
   CVectorMode mode;

private:
   UStaticMeshComponent *pointAMesh;
   UStaticMeshComponent *pointBMesh;
   UStaticMeshComponent *lineMesh;
   UStaticMeshComponent *arrowheadMesh;

// Functions -----------------------------------------------------------------------------------------------------------------------------------------

public:
// Constructing Vector Setup--------------------------------------------------------------------------------------------------------------------------

   void Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FVector inA, FVector inB, CVectorMode inMode = CVectorMode::general, FName inName = "");

   UFUNCTION(BlueprintCallable, Category = "math lab|constructingVector")
   void SetComponents(TArray<UStaticMeshComponent *> laserComponents, UTextRenderComponent *inText);

// Update Functions ----------------------------------------------------------------------------------------------------------------------------------

   /**Called, when the objects need to update the position or other vales*/
   UFUNCTION(BlueprintCallable, Category = "math lab|constructingVector")
   virtual void Update() override;

   /**Sets up the Components to display the line in the selected mode*/
   UFUNCTION(BlueprintCallable, Category = "math lab|constructingVector")
   void BuildCVector();

   /**Moves PointB to the defined coordinate*/
   UFUNCTION(BlueprintCallable, Category = "math lab|constructingVector")
   void MovePointB();

// Setting Functions ---------------------------------------------------------------------------------------------------------------------------------

   UFUNCTION(BlueprintCallable, Category = "math lab|constructingVector|visibility")
   void SetVisibilityPointA(bool visibility);
   UFUNCTION(BlueprintCallable, Category = "math lab|constructingVector|visibility")
   void SetVisibilityPointB(bool visibility);
   UFUNCTION(BlueprintCallable, Category = "math lab|constructingVector|visibility")
   void SetVisibilityLine(bool visibility);
   UFUNCTION(BlueprintCallable, Category = "math lab|constructingVector|visibility")
   void SetVisibilityArrowhead(bool visibility);
   UFUNCTION(BlueprintCallable, Category = "math lab|constructingVector|visibility")
   void SetVisibilityForAll(bool visibility);
   UFUNCTION(BlueprintCallable, Category = "math lab|constructingVector|visibility")
   void SetVisibility(bool showPointA, bool showPointB, bool showLine, bool showArrowhead);

   /* Sets nwe values to the constructing vector. */
   UFUNCTION(Blueprintcallable, Category = "math lab|constructingVector")
   void SetCVector(FVector inA, FVector inB);

// Name Functions-------------------------------------------------------------------------------------------------------------------------------------

   virtual void SetDefaultNameVisibility() override;

};

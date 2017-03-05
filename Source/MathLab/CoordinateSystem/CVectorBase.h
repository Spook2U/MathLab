// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/GeometryBase.h"
#include "CVectorBase.generated.h"



UENUM(BlueprintType)
enum class CVectorMode : uint8
{
   point        UMETA(DisplayName = "Point"),
   segment      UMETA(DisplayName = "Segment"),
   vector       UMETA(DisplayName = "Vector"),
   vectorPoint  UMETA(DisplayName = "Vector to Point"),
   general      UMETA(DisplayName = "General")
};



UCLASS()
class MATHLAB_API ACVectorBase : public AGeometryBase
{
	GENERATED_BODY()
	
public:
   ACVectorBase();

public:
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

// -------------------------------------------------------------------------------------------------

public:
   UFUNCTION(BlueprintCallable, Category = "math lab|constructingVector")
   void SetComponents(TArray<UStaticMeshComponent *> laserComponents, UTextRenderComponent *inText);

   void Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FVector inA, FVector inB, CVectorMode inMode = CVectorMode::general, FString inName = "");

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

   /**Called, when the objects need to update the position or other vales*/
   UFUNCTION(BlueprintCallable, Category = "math lab|constructingVector")
   virtual void Update() override;

   /**Sets up the Components to display the line in the selected mode*/
   UFUNCTION(BlueprintCallable, Category = "math lab|constructingVector")
   void BuildCVector();

   /**Moves PointB to the defined coordinate*/
   UFUNCTION(BlueprintCallable, Category = "math lab|constructingVector")
   void MovePointB();

protected:
   virtual void ShowText() override;
};

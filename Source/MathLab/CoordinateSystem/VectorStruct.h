// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/GeometryBase.h"
#include "VectorStruct.generated.h"



UENUM(BlueprintType)
enum class VectorStructMode : uint8
{
   point        UMETA(DisplayName = "Point"),
   segment      UMETA(DisplayName = "Segment"),
   vector       UMETA(DisplayName = "Vector"),
   vectorPoint  UMETA(DisplayName = "Vector to Point"),
   general      UMETA(DisplayName = "General")
};



UCLASS()
class MATHLAB_API AVectorStruct : public AGeometryBase
{
	GENERATED_BODY()
	
public:
   AVectorStruct();

public:
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "vectorStruct")
   FVector a;
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "vectorStruct")
   FVector b;
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "vectorStruct")
   VectorStructMode mode;

private:
   UStaticMeshComponent *pointAMesh;
   UStaticMeshComponent *pointBMesh;
   UStaticMeshComponent *lineMesh;
   UStaticMeshComponent *arrowheadMesh;

// -------------------------------------------------------------------------------------------------

public:
   UFUNCTION(BlueprintCallable, Category = "math lab|vectorStruct")
   void SetComponents(TArray<UStaticMeshComponent *> laserComponents, UTextRenderComponent *inText);

   void Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FVector inA, FVector inB, VectorStructMode inMode = VectorStructMode::general, FText inName = FText::FromString(""));

   UFUNCTION(BlueprintCallable, Category = "math lab|vectorStruct|visibility")
   void SetVisibilityPointA(bool visibility);
   UFUNCTION(BlueprintCallable, Category = "math lab|vectorStruct|visibility")
   void SetVisibilityPointB(bool visibility);
   UFUNCTION(BlueprintCallable, Category = "math lab|vectorStruct|visibility")
   void SetVisibilityLine(bool visibility);
   UFUNCTION(BlueprintCallable, Category = "math lab|vectorStruct|visibility")
   void SetVisibilityArrowhead(bool visibility);
   UFUNCTION(BlueprintCallable, Category = "math lab|vectorStruct|visibility")
   void SetVisibilityForAll(bool visibility);

   UFUNCTION(BlueprintCallable, Category = "math lab|vectorStruct|visibility")
   void SetVisibility(bool showPointA, bool showPointB, bool showLine, bool showArrowhead);

   /**Called, when the objects need to update the position or other vales*/
   UFUNCTION(BlueprintCallable, Category = "math lab|vectorStruct")
   virtual void Update() override;

   /**Sets up the Components to display the line in the selected mode*/
   UFUNCTION(BlueprintCallable, Category = "math lab|vectorStruct")
   void BuildLine();

   /**Moves PointB to the defined coordinate*/
   UFUNCTION(BlueprintCallable, Category = "math lab|vectorStruct")
   void MovePointB();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/GeometryBase.h"
#include "LineBase.generated.h"

UCLASS()
class MATHLAB_API ALineBase : public AGeometryBase
{
	GENERATED_BODY()

public:
   ALineBase();
   
// Unreal Callbacks---------------------------------------------------------------------------------
   virtual void BeginPlay() override;



////////////////////////////////////////////////////////////////////////////////////////////////////
// Member Variable  ////////////////////////////////////////////////////////////////////////////////
public:
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "line")
   FVector Position;
   
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "line")
   FVector Direction;
   
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "line")
   LineMode Mode;


private:
   UStaticMeshComponent *Line;
   UStaticMeshComponent *Arrowhead;



////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions  //////////////////////////////////////////////////////////////////////////////////////
public:
// Initialise --------------------------------------------------------------------------------------

   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "coordinate System|line|Init")
   void SetComponents(UStaticMeshComponent *line, UStaticMeshComponent *arrowHead, TArray<UStaticMeshComponent *> laserComponents);

// Update -------------------------------------------------------------------------------------------

   //Called, when the objects need to update the position or other vales
   UFUNCTION(BlueprintCallable, Category = "coordinate System|line|Update")
   virtual void Update() override;

// Setup --------------------------------------------------------------------------------------------

   //Sets up the Components to display the line in the selected mode
   UFUNCTION(BlueprintCallable, Category = "coordinate System|line|Setup")
   void BuildLine();

// -------------------------------------------------------------------------------------------------

   void SetValuesLine(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector position, FVector direction, LineMode mode);
   



// -------------------------------------------------------------------------------------------------
protected:
   virtual void CreateGuides(LaserColors color) override;

   void ScaleLaser(float length);

   void MoveLaser(UStaticMeshComponent *laser, float length);

};

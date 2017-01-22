// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MathLabTools.h"
#include "CoordinateSystemBase.h"
#include "GameFramework/Actor.h"
#include "GeometryBase.generated.h"

UCLASS()
class MATHLAB_API AGeometryBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AGeometryBase();
   
   virtual void BeginPlay() override;

   // Test Debug Print Function
   UFUNCTION(BlueprintCallable, Displayname = "Print (DebugOnly)", Category = "string", meta = (Keywords = "mld, print, debug, printdebug", Tooltip = "This method is only working in Debug or Development Mode"))
   void bp_debug_Screen(FString inString = "Hello", FLinearColor color = FLinearColor::White);



public:
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   ACoordinateSystemBase *CoordinateSystem;

   //Saves all Components, which have a LaserMaterial
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   TArray<UStaticMeshComponent *> LaserCompoents;

   //Saves all objects, which are used a guides for the objcet
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   TArray<AGeometryBase *> Guides;

   //If true, this object is a guide of another object, therefore dont have it's own guides
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   bool IsGuide;

   //Used to determine the size of the object
   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "geometry")
   float Size;

// -------------------------------------------------------------------------------------------------

public:
   //Converts the given Coordinate to the Location in the Scene
   UFUNCTION(BlueprintPure, Category = "coordinate System|geometry|Util|")
   FVector CoordinateToLocation(FVector coordinate);

   //Called, when the objects need to update the position or other vales
   virtual void Update();

   //Updates the visible area of the Material
   UFUNCTION(BlueprintCallable, Category = "coordinate System|geometry|Update")
   void UpdateRendering();

   //Sets Location of the object based on the Coordinate
   UFUNCTION(BlueprintCallable, Category = "coordinate System|geometry|Setup")
   void SetPosition(FVector coordinate);

   //Sets Color and Glowiness depending to the enum value and changes the Material of all Laser Components in the array
   UFUNCTION(BlueprintCallable, Category = "coordinate System|geometry|Setup")
   void SetColor(LaserColors color);

   //Shows or Hides the guide objects
   UFUNCTION(BlueprintCallable, Category = "coordinate System|geometry")
   void ShowGuides(bool show);

   void SetValues(ACoordinateSystemBase *coordinateSystem, LaserColors color);
   


protected:
   void AddGuide(AGeometryBase *guide);

   void AddLaserComponent(UStaticMeshComponent *laser);

   virtual void CreateGuides(LaserColors color);

   void InitScalePoint(UStaticMeshComponent *point);
   void InitScaleLine(UStaticMeshComponent *line);
   void InitScaleArrowhead(UStaticMeshComponent *arrowhead);

   void SetLaserMatTransparency(UStaticMeshComponent *plane, float value);
};

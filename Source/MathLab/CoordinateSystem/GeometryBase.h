// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MathLabEnums.h"

#include "GameFramework/Actor.h"
#include "GeometryBase.generated.h"

class ACoordinateSystemBase;
class AVectorStruct;



enum class GeometryType : uint8
{
   circle,
   point,
   unit,
   line,
   plane,
   sphere,
   vectorStruct,
   other
};

enum class Direction : uint8
{
   forward,
   right,
   up
};



UCLASS()
class MATHLAB_API AGeometryBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AGeometryBase();
   
   virtual void BeginPlay() override;

public:
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   ACoordinateSystemBase *CoordinateSystem;

   //Saves all Components, which have a LaserMaterial
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   TArray<UStaticMeshComponent *> LaserCompoents;

   //Saves all objects, which are used a guides for the objcet
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   TArray<AVectorStruct *> VectorGuides;

   //Used to determine the size of the object
   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "geometry")
   float Size;

   GeometryType type;

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
   void ShowVectorGuides(bool show);

   void SetValuesGeometry(ACoordinateSystemBase *coordinateSystem, LaserColors color);
   


protected:
   void AddVectorGuide(AVectorStruct *vectorGuide);
   void AddLaserComponent(UStaticMeshComponent *laser);

   virtual void CreateVectorGuides(LaserColors color);

   

   // Component Setup Library workaround
   void InitScalePoint(UStaticMeshComponent *point);
   void InitScaleLine(UStaticMeshComponent *line);
   void InitScaleArrowhead(UStaticMeshComponent *arrowhead);

   void SetLaserMatTransparency(UStaticMeshComponent *laser, float value);
   void MoveLaser(UStaticMeshComponent *laser, Direction dir, float length);
   void RotateLaserLookAt(FVector from, FVector to);
   /* When x, y or z should not be changed use NULL. Value 0 not possible as new scale*/
   void SetLaserScale(UStaticMeshComponent *laser, FVector scale);

   void RotateLine(FVector direction);

   void ScaleLine(UStaticMeshComponent *line, float length);
   void ScaleVector(UStaticMeshComponent *line, UStaticMeshComponent *arrowhead, float lenght);
   void ScalePlane(UStaticMeshComponent *plane, float lenght);
   void ScaleSphere(UStaticMeshComponent *sphere, float radius);
};

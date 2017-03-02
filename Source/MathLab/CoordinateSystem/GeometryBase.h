// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MathLabEnums.h"

#include "GameFramework/Actor.h"
//#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "GeometryBase.generated.h"

class ACoordinateSystemBase;
class AVectorStruct;



UENUM(BlueprintType)
enum class GeometryType : uint8
{
   circle UMETA(DisplayName = "Circle"),
   line   UMETA(DisplayName = "Line"),
   plane  UMETA(DisplayName = "Plane"),
   point  UMETA(DisplayName = "Point"),
   sphere UMETA(DisplayName = "Sphere"),
   unit   UMETA(DisplayName = "Unit"),
   vectorStruct UMETA(DisplayName = "VectorStruct"),
   other   UMETA(DisplayName = "other")
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
   ACoordinateSystemBase *coordinateSystem;

   //Saves all Components, which have a LaserMaterial
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   TArray<UStaticMeshComponent *> laserCompoents;

   //Saves all objects, which are used a guides for the objcet
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   TArray<AVectorStruct *> vectorGuides;

   //Used to determine the size of the object
   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "geometry")
   float size;

   //Saves the color of the laser
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   LaserColors color;

   //Saves type of the geometry to determine the correct cast
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   GeometryType type;

   // determines if the constructig Vectors should be shown
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   bool showConstruction;


//protected:
//   UTextRenderComponent nameText;

// -------------------------------------------------------------------------------------------------

public:
   //Converts the given Coordinate to the Location in the Scene
   UFUNCTION(BlueprintPure, Category = "math lab|geometry")
   FVector CoordinateToLocation(FVector coordinate);

   void Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor);

   //Called, when the objects need to update the position or other vales
   virtual void Update();
   //Updates the visible area of the Material
   UFUNCTION(BlueprintCallable, Category = "math lab|geometry")
   void UpdateRendering();

   //Sets Color and Glowiness depending to the enum value and changes the Material of all Laser Components in the array
   UFUNCTION(BlueprintCallable, Category = "math lab|geometry")
   void SetColor(LaserColors inColor);
   //Sets Location of the object based on the Coordinate
   UFUNCTION(BlueprintCallable, Category = "math lab|geometry")
   void SetPosition(FVector coordinate);
   //Shows or Hides the guide objects
   UFUNCTION(BlueprintCallable, Category = "math lab|geometry")
   void ShowVectorGuides(bool show);

   
   virtual FString ToString();

// -------------------------------------------------------------------------------------------------

protected:
   virtual void CreateVectorGuides(LaserColors color);
   void AddVectorGuide(AVectorStruct *vectorGuide);
   void AddLaserComponent(UStaticMeshComponent *laser);

// -------------------------------------------------------------------------------------------------

   // Component Setup Library workaround
   void InitScalePoint(UStaticMeshComponent *point);
   void InitScaleLine(UStaticMeshComponent *line);
   void InitScaleArrowhead(UStaticMeshComponent *arrowhead);

   void SetLaserMatTransparency(UStaticMeshComponent *laser, float value);

   void MoveLaser(UStaticMeshComponent *laser, Direction dir, float length);

   void RotateLine(FVector direction);
   void RotateLaserLookAt(FVector from, FVector to);

   /* When x, y or z should not be changed use NULL. Value 0 not possible as new scale*/
   void SetLaserScale(UStaticMeshComponent *laser, FVector scale);
   void ScaleLine(UStaticMeshComponent *line, float length);
   void ScaleVector(UStaticMeshComponent *line, UStaticMeshComponent *arrowhead, float lenght);
   void ScalePlane(UStaticMeshComponent *plane, float lenght);
   void ScaleSphere(UStaticMeshComponent *sphere, float radius);
};

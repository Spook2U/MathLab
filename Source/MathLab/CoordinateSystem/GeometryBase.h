// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MathLabEnums.h"

#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "GeometryBase.generated.h"

class ACoordinateSystemBase;
class ACVectorBase;


// Enum Types-----------------------------------------------------------------------------------------------------------------------------------------


UENUM(BlueprintType)
enum class GeometryType : uint8
{
   circle  UMETA(DisplayName = "Circle"),
   line    UMETA(DisplayName = "Line"),
   plane   UMETA(DisplayName = "Plane"),
   point   UMETA(DisplayName = "Point"),
   sphere  UMETA(DisplayName = "Sphere"),
   unit    UMETA(DisplayName = "Unit"),
   cVector UMETA(DisplayName = "Constructing Vector"),
   other   UMETA(DisplayName = "other")
};

enum class Direction : uint8
{
   forward,
   right,
   up
};


// ---------------------------------------------------------------------------------------------------------------------------------------------------


UCLASS()
class MATHLAB_API AGeometryBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AGeometryBase();
   
   virtual void BeginPlay() override;
   virtual void Tick(float DeltaTime) override;

public:
// Member --------------------------------------------------------------------------------------------------------------------------------------------

   /* Holds a reference to the coordinatesystem in wich it's created. */
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   ACoordinateSystemBase *coordinateSystem;
   /* Holds all Components, which have a LaserMaterial. */
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   TArray<UStaticMeshComponent *> laserCompoents;
   /* Holds all objects, which are used a guides for the objcet. */
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   TArray<ACVectorBase *> constVectors;
   /* Saves the color of the laser. */
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   LaserColors color;
   /* Saves type of the geometry to determine the correct cast. */
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   GeometryType type;
   /* Determines if the constructing Vectors should be shown. */
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   bool showConstruction;

   /* Used to determine the size of the object. */
   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "geometry")
   float size;



protected:
   UTextRenderComponent *nameRender;

   FName name;
   FName nameMathData;
   bool showName;
   bool showMathData;

// Function ------------------------------------------------------------------------------------------------------------------------------------------

public:

// Geometry Setup ------------------------------------------------------------------------------------------------------------------------------------

   void Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FName inName = "");
   /* Add the StaticMeshComponent in the list of components handled as laser. */
   void AddLaserComponent(UStaticMeshComponent *laser);

// Update Functions ----------------------------------------------------------------------------------------------------------------------------------

   /* Called, when the objects need to update the position or other vales. */
   virtual void Update();
   /* Updates the visible area of the Material. */
   UFUNCTION(BlueprintCallable, Category = "math lab|geometry")
   void UpdateRendering();
   /* Moves the object to the specified coordinate. */
   UFUNCTION(Blueprintcallable, Category = "math lab|geometry")
   void Move(FVector coordinate);

// Setting Functions ---------------------------------------------------------------------------------------------------------------------------------

   /* Determines if the constructing vectors should be shown. */
   UFUNCTION(BlueprintCallable, Category = "math lab|geometry")
   void ShowConstructingVector(bool show);

   /* Sets color and glowiness depending to the enum value and changes the material of all laser components of this object. */
   UFUNCTION(BlueprintCallable, Category = "math lab|geometry")
   void SetColor(LaserColors inColor);

   /* Sets the name of the object. */
   UFUNCTION(BlueprintCallable, Category = "math lab|geometry")
   virtual void SetName(FName inName);
   /* Sets the name of the object to default. */
   UFUNCTION(BlueprintCallable, Category = "math lab|geometry")
   void ClearName();
   /* Returns the name of the object. */
   UFUNCTION(BlueprintCallable, Category = "math lab|geometry")
   FName GetName();

// Name Functions-------------------------------------------------------------------------------------------------------------------------------------

   void InitName(FName inName);

   /* Checks if inName is already used, if so returns a generaic name. */
   FName NameCheck(FName inName);

   /* Checks if inName has a value, if not creates a generaic name. 
      When showName is true a name is generated.
      When shoeMathData is true the mathematical data will be showm. */
   FName BuildName(FName inName);

   /* Sets the default value for the SetDefaultNameVisibility functions. */
   virtual void SetDefaultNameVisibility();
   /* Shows or Hides the Name of the Object. */
   UFUNCTION(BlueprintCallable, Category = "math lab|geometry")
   void SetNameVisible(bool show);
   /* Shows or Hides the mathematical Data on the Name. */
   UFUNCTION(BlueprintCallable, Category = "math lab|geometry")
   void SetMathDataVisible(bool show);
   /* Shows or Hides the Names of the constructing vectors. */
   UFUNCTION(BlueprintCallable, Category = "math lab|geometry")
   void SetCVectorNameVisible(bool show);
   /* Shows or Hides the mathematical Data of the constructing vectors. */
   UFUNCTION(BlueprintCallable, Category = "math lab|geometry")
   void SetCVectorMathDataVisible(bool show);

   /* Sets the name visible if eather showName or showMathData is true. */
   void UpdateTextVisibility();

// Utility Functions----------------------------------------------------------------------------------------------------------------------------------

   /* Converts the given Coordinate to the Location in the Scene. */
   UFUNCTION(BlueprintPure, Category = "math lab|geometry")
   FVector CoordinateToLocation(FVector coordinate);

   virtual FString ToString();

// Constructing Vector Functions ---------------------------------------------------------------------------------------------------------------------

   /* Virtual function to be overriden by children. Creates the constructing vectors. */
   virtual void CreateCVector(LaserColors inColor);
   /* Add the constructing vector to the array. */
   void AddCVector(ACVectorBase *vectorGuide);



// ---------------------------------------------------------------------------------------------------------------------------------------------------
// Cannt use static function in BlueprintFunctionLibrary Workaround
// ToDo: Move functions into MathLabLibraray and make static
// ---------------------------------------------------------------------------------------------------------------------------------------------------

// StaticMeshComponent Setup -------------------------------------------------------------------------------------------------------------------------

   void InitScalePoint(UStaticMeshComponent *point);
   void InitScaleLine(UStaticMeshComponent *line);
   void InitScaleArrowhead(UStaticMeshComponent *arrowhead);

   void SetLaserMatTransparency(UStaticMeshComponent *laser, float value);

   void MoveLaser(UStaticMeshComponent *laser, Direction dir, float length);
   void MoveText(UTextRenderComponent *textRender, FVector coordinate);

   void RotateLine(FVector direction);
   void RotateLaserLookAt(FVector from, FVector to);
   void RotateText();

   // If x, y or z should not be changed use NULL. Value 0 not possible as new scale.
   void SetLaserScale(UStaticMeshComponent *laser, FVector scale);
   void ScaleLine(UStaticMeshComponent *line, float length);
   void ScaleVector(UStaticMeshComponent *line, UStaticMeshComponent *arrowhead, float lenght);
   void ScalePlane(UStaticMeshComponent *plane, float lenght);
   void ScaleSphere(UStaticMeshComponent *sphere, float radius);
};

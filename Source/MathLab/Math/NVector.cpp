
#include "MathLab.h"
#include "NVector.h"



FNVector::FNVector() {}
FNVector::FNVector(TArray<float> inCoordinates) : Coordinates(inCoordinates) {}

// -------------------------------------------------------------------------------------------------

void FNVector::Init(int dimension)
{
   for(int i = 0; i < dimension; i++)
   {
      Coordinates.Add(0);
   }
}

bool FNVector::ValidCheck(FNVector inNVector) const
{
   bool valid = true;

   if(Size() != inNVector.Size())
   {
      valid = false;
      MLD_ERR("Invalid input nVector; Size %d expected, Invalid size: %d", Size(), inNVector.Size());
   }

   return valid;
}

int FNVector::Size() const
{
   return Coordinates.Num();
}

// -------------------------------------------------------------------------------------------------

FNVector& FNVector::operator=(const FNVector& Other)
{
   this->Coordinates = Other.Coordinates;

   return *this;
}


FNVector FNVector::operator+(const FNVector& V) const
{
   FNVector result = FNVector(Coordinates);

   if(ValidCheck(V))
   { 
      for(int i = 0; i < Size(); i++)
      {
         result.Coordinates[i] += V.Get(i);
      }
   }

   return result;
}

FNVector FNVector::operator-(const FNVector& V) const
{
   FNVector result = FNVector(Coordinates);

   if(ValidCheck(V))
   { 
      for(int i = 0; i < Size(); i++)
      {
         result.Coordinates[i] -= V.Get(i);
      }
   }

   return result;
}

FNVector FNVector::operator*(float Scale) const
{
   FNVector result = FNVector(Coordinates);

   for(int i = 0; i < Size(); i++)
   {
      result.Coordinates[i] *= Scale;
   }

   return result;

}

FNVector FNVector::operator/(float Scale) const
{
   FNVector result = FNVector(Coordinates);

   if(Scale != 0)
   {
      for(int i = 0; i < Size(); i++)
      {
         result.Coordinates[i] /= Scale;
      }
   }
   else
   {
      MLD_ERR("Division through 0 not allowed!");
   }

   return result;
}

//FNVector FNVector::operator*(const FNVector& V) const {}
//FNVector FNVector::operator/(const FNVector& V) const {}

bool FNVector::operator==(const FNVector& V) const
{
   bool equal = true; 

   if(ValidCheck(V))
   { 
      for(int i = 0; i < Size(); i++)
      {
         if(Get(i) != V.Get(i))
         {
            equal = false;
            break;
         }
      }
   }

   return equal;
}

bool FNVector::operator!=(const FNVector& V) const
{
   bool notEqual = false; 

   if(ValidCheck(V))
   { 
      for(int i = 0; i < Size(); i++)
      {
         if(Get(i) != V.Get(i))
         {
            notEqual = true;
            break;
         }
      }
   }

   return notEqual;
}

FNVector FNVector::operator+=(const FNVector& V)
{
   if(ValidCheck(V))
   { 
      for(int i = 0; i < Size(); i++)
      {
         Coordinates[i] += V.Get(i);
      }
   }

   return *this;
}

FNVector FNVector::operator-=(const FNVector& V)
{
   if(ValidCheck(V))
   { 
      for(int i = 0; i < Size(); i++)
      {
         Coordinates[i] -= V.Get(i);
      }
   }


   return *this;
}

FNVector FNVector::operator*=(float Scale)
{
   for(int i = 0; i < Size(); i++)
   {
      Coordinates[i] *= Scale;
   }

   return *this;
}

FNVector FNVector::operator/=(float Scale)
{
   if(Scale != 0)
   {
      for(int i = 0; i < Size(); i++)
      {
         Coordinates[i] /= Scale;
      }
   }
   else
   {
      MLD_ERR("Division through 0 not allowed!");
   }

   return *this;
}

//FNVector FNVector::operator*=(const FNVector& V) {}
//FNVector FNVector::operator/=(const FNVector& V) {}

// -------------------------------------------------------------------------------------------------

FVector FNVector::GetFVector()
{
   float x = 0, y = 0, z = 0;
   switch(Size())
   {
      default: 
      case 3: z = Coordinates[2];
      case 2: y = Coordinates[1];
      case 1: x = Coordinates[0];
      case 0: break;
   }

   return FVector(x, y, z);
}

float FNVector::Get(int index) const
{
   float coordinate = 0;

   if((index < 0) || (index > Size())) { MLD_ERR("nVector out of Bounds: index = %d", index); }
   else { coordinate = Coordinates[index]; }

   return coordinate;
}

void FNVector::Set(int index, float value) 
{
   if((index < 0) || (index > Size())) { MLD_ERR("nVector out of Bounds: index = %d", index); }
   else { Coordinates[index] = value; }
}

void FNVector::Set(FNVector inNVector)
{
   Coordinates = inNVector.Coordinates;
}

void FNVector::Set(TArray<float> coordinates) 
{
   Set(FNVector(coordinates));
}

void FNVector::Add(float value) 
{
   Coordinates.Add(value);
}

void FNVector::Append(TArray<float> coordinates)
{
   for(float f : coordinates)
   {
      Add(f);
   }
}

void FNVector::RemoveAt(int index) 
{
   if((index < 0) || (index > Size())) { MLD_ERR("nVector out of Bounds: index = %d", index); }
   else { Coordinates.RemoveAt(index); }
}

// -------------------------------------------------------------------------------------------------

FString FNVector::ToString() const
{
   FString s = "";

   s += "{";
   for(int i = 0, max = Coordinates.Num(); i < max; i++)
   {
      if(Coordinates[i] < 10 && Coordinates[i] > -10)
      {
         s += " ";
      }
      if(Coordinates[i] >= 0) 
      { 
         s += " "; 
      }
      s += FString::SanitizeFloat(Coordinates[i]);
      if(i < (max - 1))      
      { 
         s += ", "; 
      }
   }
   s += "}";

   return s;
}

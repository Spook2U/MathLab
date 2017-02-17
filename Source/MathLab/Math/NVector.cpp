
#include "MathLab.h"
#include "NVector.h"



FNVector::FNVector() {}
FNVector::FNVector(TArray<float> inCoordinates) : coordinates(inCoordinates) {}

// -------------------------------------------------------------------------------------------------

void FNVector::Init(int dimension)
{
   for(int i = 0; i < dimension; i++)
   {
      coordinates.Add(0);
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
   return coordinates.Num();
}

// -------------------------------------------------------------------------------------------------

FNVector& FNVector::operator=(const FNVector& Other)
{
   this->coordinates = Other.coordinates;

   return *this;
}


FNVector FNVector::operator+(const FNVector& V) const
{
   FNVector result = FNVector(coordinates);

   if(ValidCheck(V))
   { 
      for(int i = 0; i < Size(); i++)
      {
         result.coordinates[i] += V.Get(i);
      }
   }

   return result;
}

FNVector FNVector::operator-(const FNVector& V) const
{
   FNVector result = FNVector(coordinates);

   if(ValidCheck(V))
   { 
      for(int i = 0; i < Size(); i++)
      {
         result.coordinates[i] -= V.Get(i);
      }
   }

   return result;
}

FNVector FNVector::operator*(float Scale) const
{
   FNVector result = FNVector(coordinates);

   for(int i = 0; i < Size(); i++)
   {
      result.coordinates[i] *= Scale;
   }

   return result;

}

FNVector FNVector::operator/(float Scale) const
{
   FNVector result = FNVector(coordinates);

   if(Scale != 0)
   {
      for(int i = 0; i < Size(); i++)
      {
         result.coordinates[i] /= Scale;
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
         coordinates[i] += V.Get(i);
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
         coordinates[i] -= V.Get(i);
      }
   }


   return *this;
}

FNVector FNVector::operator*=(float Scale)
{
   for(int i = 0; i < Size(); i++)
   {
      coordinates[i] *= Scale;
   }

   return *this;
}

FNVector FNVector::operator/=(float Scale)
{
   if(Scale != 0)
   {
      for(int i = 0; i < Size(); i++)
      {
         coordinates[i] /= Scale;
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
      case 3: z = coordinates[2];
      case 2: y = coordinates[1];
      case 1: x = coordinates[0];
      case 0: break;
   }

   return FVector(x, y, z);
}

float FNVector::Get(int index) const
{
   float coordinate = 0;

   if((index < 0) || (index > Size())) { MLD_ERR("nVector out of Bounds: index = %d", index); }
   else { coordinate = coordinates[index]; }

   return coordinate;
}

void FNVector::Set(int index, float value) 
{
   if((index < 0) || (index > Size())) { MLD_ERR("nVector out of Bounds: index = %d", index); }
   else { coordinates[index] = value; }
}

void FNVector::Set(FNVector inNVector)
{
   coordinates = inNVector.coordinates;
}

void FNVector::Set(TArray<float> inCoordinates) 
{
   Set(FNVector(inCoordinates));
}

void FNVector::Add(float value) 
{
   coordinates.Add(value);
}

void FNVector::Append(TArray<float> inCoordinates)
{
   for(float f : inCoordinates)
   {
      Add(f);
   }
}

void FNVector::RemoveAt(int index) 
{
   if((index < 0) || (index > Size())) { MLD_ERR("nVector out of Bounds: index = %d", index); }
   else { coordinates.RemoveAt(index); }
}

// -------------------------------------------------------------------------------------------------

FString FNVector::ToString() const
{
   FString s = "";

   s += "{";
   for(int i = 0, max = coordinates.Num(); i < max; i++)
   {
      if(coordinates[i] < 10 && coordinates[i] > -10)
      {
         s += " ";
      }
      if(coordinates[i] >= 0) 
      { 
         s += " "; 
      }
      s += FString::SanitizeFloat(coordinates[i]);
      if(i < (max - 1))      
      { 
         s += ", "; 
      }
   }
   s += "}";

   return s;
}

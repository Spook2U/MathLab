#include "MathLab.h"
#include "DebugTools.h"
#include "CoordinateSystem/CoordinateSystemBase.h"
#include "Engine.h"

ADebugTools::ADebugTools()  {}
ADebugTools::~ADebugTools() {}

#ifdef _UE_BUILD_DEBUG_FLAG_
DEFINE_LOG_CATEGORY(MLogCode);
DEFINE_LOG_CATEGORY(MLogBlue);
DEFINE_LOG_CATEGORY(MLogScrn);



FString debug_ClassPrefix(FString classname)
{
   FString prefix = "";

   if(classname.Equals(TEXT("CoordinateSystemBase.cpp")))   { prefix = "CSyst - "; }
   else if(classname.Equals(TEXT("GeometryBase.cpp")))      { prefix = "Geo   - "; }
   else if(classname.Equals(TEXT("PointBase.cpp")))         { prefix = "Point - "; }
   else if(classname.Equals(TEXT("UnitBase.cpp")))          { prefix = "Unit  - "; }
   else if(classname.Equals(TEXT("LineBase.cpp")))          { prefix = "Line  - "; }
   else if(classname.Equals(TEXT("PlaneBase.cpp")))         { prefix = "Plane - "; }
   else if(classname.Equals(TEXT("SphereBase.cpp")))        { prefix = "Spher - "; }
   else if(classname.Equals(TEXT("LinearEqualation.cpp")))  { prefix = "LinEq - "; }
   else if(classname.Equals(TEXT("NVector.cpp")))           { prefix = "nVect - "; }
   else if(classname.Equals(TEXT("DebugTools.cpp")))        { prefix = ""; }
   else                                                     { prefix = classname + " - "; }

   return prefix;
}

FString debug_BuildText(FString classname, FString ausgabe)
{
   FString result = FString::Printf(TEXT("%s%s"), *debug_ClassPrefix(classname), *ausgabe);
   return result;
}



void doTest(bool valid, LPCSTR file, int line, LPCSTR func, LPCSTR name)
{
   if(!valid)
   {
      MLD_ERR("%s%s = NULL, %s() (line %d)", *debug_ClassPrefix(file), *FString(name), *FString(func), line);
      g_this->EndPlay(EEndPlayReason::Quit);
      //EndPlay()    
   }
}

void debug_uescreen(FString classname, FString ausgabe, FColor color, int mode)
{
   GEngine->AddOnScreenDebugMessage(-1, 30, color, ausgabe);
   switch(mode)
   {
      case MLog::Blue: UE_LOG(MLogBlue, Log, TEXT("%s"), *debug_BuildText(classname, ausgabe)); break;
      case MLog::Scrn: UE_LOG(MLogScrn, Log, TEXT("%s"), *debug_BuildText(classname, ausgabe)); break;
      default:         UE_LOG(MLogCode, Log, TEXT("%s"), *debug_BuildText(classname, ausgabe)); break;
   }
}


#endif

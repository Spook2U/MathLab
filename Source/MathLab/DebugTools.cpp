#include "MathLab.h"
#include "DebugTools.h"
#include "CoordinateSystem/CoordinateSystemBase.h"
#include "Engine.h"

ADebugTools::ADebugTools()  {}
ADebugTools::~ADebugTools() {}

#ifdef _UE_BUILD_DEBUG_FLAG_
void doTest(bool valid, LPCSTR file, int line, LPCSTR func, LPCSTR name)
{
   if(!valid)
   {
      MLD_ERR("%s - %s = NULL %s() (line %d)", *debug_ClassPrefix(file), *FString(name), *FString(func), line);
      g_this->EndPlay(EEndPlayReason::Quit);
      //EndPlay()    
   }
}


DEFINE_LOG_CATEGORY(MLogCode);
DEFINE_LOG_CATEGORY(MLogBlue);
DEFINE_LOG_CATEGORY(MLogScrn);

void debug_uescreen(FString s, FColor color, int mode)
{
   GEngine->AddOnScreenDebugMessage(-1, 30, color, s);
   switch(mode)
   {
      case MLog::Blue: UE_LOG(MLogBlue, Log, TEXT("%s - %s"), *debug_ClassPrefix(MLD_CLASS_NAME), *s); break;
      case MLog::Scrn: UE_LOG(MLogScrn, Log, TEXT("%s - %s"), *debug_ClassPrefix(MLD_CLASS_NAME), *s); break;
      default:         UE_LOG(MLogCode, Log, TEXT("%s - %s"), *debug_ClassPrefix(MLD_CLASS_NAME), *s); break;
   }
}

FString debug_ClassPrefix(FString functionname)
{
   FString prefix = "";
   
   if     (functionname.Equals(TEXT("CoordinateSystemBase.cpp"))) { prefix = "CoSys"; }
   else if(functionname.Equals(TEXT("GeometryBase.cpp")))         { prefix = "Geo  "; }
   else if(functionname.Equals(TEXT("PointBase.cpp")))            { prefix = "Point"; }
   else if(functionname.Equals(TEXT("UnitBase.cpp")))             { prefix = "Unit "; }
   else if(functionname.Equals(TEXT("LineBase.cpp")))             { prefix = "Line "; }
   else if(functionname.Equals(TEXT("PlaneBase.cpp")))            { prefix = "Plane"; }
   else if(functionname.Equals(TEXT("SphereBase.cpp")))           { prefix = "Spher"; }
   else if(functionname.Equals(TEXT("LinearEqualaton.cpp")))      { prefix = "LinEq"; }
   else                                                           { prefix = functionname; }

   return prefix;
}

#endif

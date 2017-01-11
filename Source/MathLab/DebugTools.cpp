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
      PRINTERR("%s = NULL, Line: %d in %s() - %s", *FString(name), line, *FString(func), *FString(file));
      g_this->EndPlay(EEndPlayReason::Quit);
      //EndPlay()    
   }
}


DEFINE_LOG_CATEGORY(MLog);
DEFINE_LOG_CATEGORY(MLogScreen);

void debug_uescreen(FString s, FColor color)
{
   GEngine->AddOnScreenDebugMessage(-1, 30, color, s);
   UE_LOG(MLogScreen, Log, TEXT("%s"), *s);
}

#endif
